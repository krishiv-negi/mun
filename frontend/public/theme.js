// File: frontend/public/theme.js
// Persists Dark/Light theme across all pages and adds Theme Toggle + Back to Home buttons automatically
(function () {
  const STORAGE_KEY = "mun_theme";

  function getSavedTheme() {
    const saved = localStorage.getItem(STORAGE_KEY);
    return saved === "dark" || saved === "light" ? saved : "light";
  }

  function updateButtonUI(btn, theme) {
    if (theme === "light") {
      btn.innerHTML = '<i class="ph ph-moon"></i> Dark';
      btn.title = "Switch to Dark Mode";
    } else {
      btn.innerHTML = '<i class="ph ph-sun"></i> Light';
      btn.title = "Switch to Light Mode";
    }
  }

  function applyTheme(theme) {
    document.documentElement.setAttribute("data-theme", theme);
    localStorage.setItem(STORAGE_KEY, theme);
    const btn = document.getElementById("themeToggleBtn");
    if (btn) updateButtonUI(btn, theme);
  }

  function createHomeLink() {
    const a = document.createElement("a");
    a.href = "/";
    a.id = "backToHomeBtn";
    a.className = "theme-toggle-btn";
    a.style.textDecoration = "none";
    a.title = "Back to Home Page";
    a.innerHTML = '<i class="ph ph-house"></i> Home';
    return a;
  }

  function mountControls() {
    if (document.getElementById("themeToggleBtn")) return;

    const path = window.location.pathname.toLowerCase();
    const isHomePage = path === "/" || path.endsWith("/home.html");
    const hasHomeLink = Boolean(document.querySelector('a[href="/"], a[href="/home.html"]'));
    const needHomeBtn = !isHomePage && !hasHomeLink;

    const themeBtn = document.createElement("button");
    themeBtn.type = "button";
    themeBtn.id = "themeToggleBtn";
    themeBtn.className = "theme-toggle-btn";
    updateButtonUI(themeBtn, getSavedTheme());

    themeBtn.addEventListener("click", function () {
      const current = document.documentElement.getAttribute("data-theme") || "light";
      applyTheme(current === "light" ? "dark" : "light");
    });

    // 1. Home page: place theme toggle next to #authActions (no Back to Home button needed here)
    const authActions = document.getElementById("authActions");
    if (authActions && authActions.parentElement) {
      let group = authActions.parentElement.querySelector(".nav-right-group");
      if (!group) {
        group = document.createElement("div");
        group.className = "nav-right-group";
        authActions.parentElement.insertBefore(group, authActions);
        group.appendChild(themeBtn);
        group.appendChild(authActions);
      } else {
        group.insertBefore(themeBtn, group.firstChild);
      }
      return;
    }

    // 2. Delegate portal: place Home + Dark/Light toggle inside top-right .user-bar before Logout
    const userBar = document.querySelector(".user-bar");
    if (userBar) {
      const logoutBtn = document.getElementById("btnLogout");
      if (needHomeBtn) userBar.insertBefore(createHomeLink(), logoutBtn || null);
      userBar.insertBefore(themeBtn, logoutBtn || null);
      return;
    }

    // 3. Admin pages (Dashboard, Delegates, Committees): group Home + Dark/Light toggle + Logout
    const navbar = document.querySelector(".navbar");
    const adminLogout = navbar && (navbar.querySelector("#btnLogout") || navbar.querySelector(".logout"));
    if (navbar && adminLogout) {
      const group = document.createElement("div");
      group.className = "nav-right-group";
      navbar.insertBefore(group, adminLogout);
      if (needHomeBtn) group.appendChild(createHomeLink());
      group.appendChild(themeBtn);
      group.appendChild(adminLogout);
      return;
    }

    // 4. Pages without a navbar (e.g. Login / Sign Up): float cleanly in top-right corner
    const floatWrap = document.createElement("div");
    floatWrap.className = "nav-right-group";
    floatWrap.style.cssText = "position:fixed;top:18px;right:20px;z-index:999;";
    if (needHomeBtn) floatWrap.appendChild(createHomeLink());
    floatWrap.appendChild(themeBtn);
    document.body.appendChild(floatWrap);
  }

  // Apply immediately before page paints to prevent flash
  applyTheme(getSavedTheme());

  document.addEventListener("DOMContentLoaded", mountControls);

  // Sync theme if changed in another open tab
  window.addEventListener("storage", function (e) {
    if (e.key === STORAGE_KEY && (e.newValue === "dark" || e.newValue === "light")) {
      document.documentElement.setAttribute("data-theme", e.newValue);
      const btn = document.getElementById("themeToggleBtn");
      if (btn) updateButtonUI(btn, e.newValue);
    }
  });
})();
