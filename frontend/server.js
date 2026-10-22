// server.js - web server for the MUN project
//   1. /api/analyze     -> runs the C++ cv_parser on an uploaded CV
//   2. /api/delegates   -> reads/writes the SAME MySQL database as the C++ menu program
const express = require("express");
const multer = require("multer");
const mysql = require("mysql2/promise");
const path = require("path");
const fs = require("fs");
const os = require("os");
const { execFile } = require("child_process");

const app = express();
app.use(express.json());
app.use(express.static(path.join(__dirname, "public")));

// ---------------------------------------------------------------
// Database (same settings as your C++ code). Override with env vars:
//   DB_PASSWORD=yourpass npm start
// ---------------------------------------------------------------
const pool = mysql.createPool({
  host: process.env.DB_HOST || "127.0.0.1",
  port: Number(process.env.DB_PORT || 3306),
  user: process.env.DB_USER || "root",
  password: process.env.DB_PASSWORD || "admin",
  database: process.env.DB_NAME || "mun_management",
  connectionLimit: 5,
});

// ---------------- CV analyzer ----------------
const upload = multer({ dest: os.tmpdir(), limits: { fileSize: 5 * 1024 * 1024 } });
const PARSER = path.join(__dirname, "..", "backend", "cv_parser");

function run(cmd, args) {
  return new Promise((resolve, reject) => {
    execFile(cmd, args, { maxBuffer: 10 * 1024 * 1024 }, (err, stdout, stderr) => {
      if (err) return reject(new Error(stderr || err.message));
      resolve(stdout);
    });
  });
}

app.post("/api/analyze", upload.single("cv"), async (req, res) => {
  if (!req.file) return res.status(400).json({ error: "No file uploaded." });
  const tmpFiles = [req.file.path];
  try {
    let textPath = req.file.path;
    const isPdf = /\.pdf$/i.test(req.file.originalname) || req.file.mimetype === "application/pdf";
    if (isPdf) {
      const text = await run("pdftotext", ["-layout", req.file.path, "-"]); // brew install poppler
      textPath = req.file.path + ".txt";
      fs.writeFileSync(textPath, text);
      tmpFiles.push(textPath);
    }
    const out = await run(PARSER, [textPath, "--json"]);
    res.json(JSON.parse(out));
  } catch (e) {
    res.status(500).json({ error: e.message.trim() });
  } finally {
    tmpFiles.forEach((f) => fs.unlink(f, () => {}));
  }
});

// ---------------- Delegates (MySQL) ----------------
// Tables, as used by addDelegate.cpp:
//   delegates(delegate_id, name, age, country, mun_attend)
//   previous_committees(delegate_id, committee_name)
//   award(delegate_id, award_name)

function cleanList(arr) {
  if (!Array.isArray(arr)) return [];
  return arr.map((s) => String(s ?? "").trim().slice(0, 150)).filter(Boolean).slice(0, 50);
}
function cleanInt(v) {
  const n = parseInt(v, 10);
  return Number.isFinite(n) && n >= 0 && n < 100000 ? n : null;
}
function parseDelegate(body) {
  const name = String(body.name ?? "").trim().slice(0, 100);
  const country = String(body.country ?? "").trim().slice(0, 100);
  const age = cleanInt(body.age);
  const mun = cleanInt(body.mun_attend);
  if (!name) return { error: "Name is required." };
  if (age === null || age < 1 || age > 120) return { error: "Enter a valid age." };
  if (!country) return { error: "Country is required." };
  if (mun === null) return { error: "Enter a valid number of MUNs attended." };
  return { name, age, country, mun_attend: mun,
           committees: cleanList(body.committees), awards: cleanList(body.awards) };
}

async function attachChildren(rows) {
  if (!rows.length) return rows;
  const ids = rows.map((r) => r.delegate_id);
  const [cs] = await pool.query("SELECT delegate_id, committee_name FROM previous_committees WHERE delegate_id IN (?)", [ids]);
  const [as] = await pool.query("SELECT delegate_id, award_name FROM award WHERE delegate_id IN (?)", [ids]);
  return rows.map((r) => ({
    id: r.delegate_id, name: r.name, age: r.age, country: r.country, mun_attend: r.mun_attend,
    committees: cs.filter((c) => c.delegate_id === r.delegate_id).map((c) => c.committee_name),
    awards: as.filter((a) => a.delegate_id === r.delegate_id).map((a) => a.award_name),
  }));
}

async function writeChildren(conn, id, committees, awards) {
  for (const c of committees)
    await conn.query("INSERT INTO previous_committees (delegate_id, committee_name) VALUES (?, ?)", [id, c]);
  for (const a of awards)
    await conn.query("INSERT INTO award (delegate_id, award_name) VALUES (?, ?)", [id, a]);
}

async function withTransaction(fn) {
  const conn = await pool.getConnection();
  try {
    await conn.beginTransaction();
    const out = await fn(conn);
    await conn.commit();
    return out;
  } catch (e) {
    await conn.rollback();
    throw e;
  } finally {
    conn.release();
  }
}

function dbError(res, e) {
  console.error("DB error:", e.message);
  const hint = e.code === "ER_ACCESS_DENIED_ERROR" ? "Database login failed (check DB_PASSWORD)."
             : e.code === "ECONNREFUSED" ? "Cannot reach MySQL. Is it running?"
             : "Database error: " + e.message;
  res.status(500).json({ error: hint });
}

// Display + Search
app.get("/api/delegates", async (req, res) => {
  try {
    const q = String(req.query.q || "").trim();
    let rows;
    if (q) {
      const like = "%" + q + "%";
      [rows] = await pool.query(
        `SELECT * FROM delegates d WHERE d.name LIKE ? OR d.country LIKE ? OR CAST(d.age AS CHAR) LIKE ?
           OR EXISTS (SELECT 1 FROM previous_committees pc WHERE pc.delegate_id = d.delegate_id AND pc.committee_name LIKE ?)
           OR EXISTS (SELECT 1 FROM award a WHERE a.delegate_id = d.delegate_id AND a.award_name LIKE ?)
         ORDER BY d.delegate_id`, [like, like, like, like, like]);
    } else {
      [rows] = await pool.query("SELECT * FROM delegates ORDER BY delegate_id");
    }
    res.json(await attachChildren(rows));
  } catch (e) { dbError(res, e); }
});

// Add
app.post("/api/delegates", async (req, res) => {
  const d = parseDelegate(req.body || {});
  if (d.error) return res.status(400).json({ error: d.error });
  try {
    const id = await withTransaction(async (conn) => {
      const [r] = await conn.query(
        "INSERT INTO delegates (name, age, country, mun_attend) VALUES (?, ?, ?, ?)",
        [d.name, d.age, d.country, d.mun_attend]);
      await writeChildren(conn, r.insertId, d.committees, d.awards);
      return r.insertId;
    });
    res.status(201).json({ id });
  } catch (e) { dbError(res, e); }
});

// Update
app.put("/api/delegates/:id", async (req, res) => {
  const id = Number(req.params.id);
  const d = parseDelegate(req.body || {});
  if (d.error) return res.status(400).json({ error: d.error });
  try {
    const found = await withTransaction(async (conn) => {
      const [r] = await conn.query(
        "UPDATE delegates SET name=?, age=?, country=?, mun_attend=? WHERE delegate_id=?",
        [d.name, d.age, d.country, d.mun_attend, id]);
      if (r.affectedRows === 0) {
        const [chk] = await conn.query("SELECT 1 FROM delegates WHERE delegate_id=?", [id]);
        if (!chk.length) return false;
      }
      await conn.query("DELETE FROM previous_committees WHERE delegate_id=?", [id]);
      await conn.query("DELETE FROM award WHERE delegate_id=?", [id]);
      await writeChildren(conn, id, d.committees, d.awards);
      return true;
    });
    if (!found) return res.status(404).json({ error: "Delegate not found." });
    res.json({ ok: true });
  } catch (e) { dbError(res, e); }
});

// Delete
app.delete("/api/delegates/:id", async (req, res) => {
  const id = Number(req.params.id);
  try {
    const n = await withTransaction(async (conn) => {
      await conn.query("DELETE FROM previous_committees WHERE delegate_id=?", [id]);
      await conn.query("DELETE FROM award WHERE delegate_id=?", [id]);
      const [r] = await conn.query("DELETE FROM delegates WHERE delegate_id=?", [id]);
      return r.affectedRows;
    });
    if (!n) return res.status(404).json({ error: "Delegate not found." });
    res.json({ ok: true });
  } catch (e) { dbError(res, e); }
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => console.log(`Running at http://localhost:${PORT}`));
