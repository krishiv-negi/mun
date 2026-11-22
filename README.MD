# MUN Management & Allocation System

## Project Description

Model United Nations (MUN) is an academic simulation of the United Nations in which participants represent different countries and discuss global issues.

The **MUN Management & Allocation System** is a web-based application designed to make delegate management and committee/portfolio allocation faster, more organized, and less dependent on manual work.

The system manages delegate details such as MUN experience, previous committees, awards, and preferences. It uses this information to support suitable committee and portfolio allocation while considering committee capacity and availability.

## Project Objectives

Currently, MUN delegate and committee information is often managed manually using spreadsheets, forms, and separate files. This process is time-consuming and may lead to errors such as duplicate allocations and unsuitable portfolio assignments.

The main objectives of this project are to:

- Manage delegate information efficiently.
- Extract relevant information from uploaded CVs.
- Maintain committee and portfolio details.
- Support suitable delegate allocation based on experience and availability.
- Reduce manual work and improve the organization of MUN management.

## Team Members

- **Krishiv Negi** – Team Lead
- Ayush Badoni
- Aniket Kaushal
- Arjun Rana

## Technologies & Tools Used

- **Programming Languages:** C, C++, JavaScript
- **Frontend:** HTML, CSS, JavaScript
- **API / Backend:** Node.js, Express.js
- **Core Processing:** C++
- **Database:** MySQL
- **Communication:** JSON API
- **IDE:** Visual Studio Code
- **Version Control:** Git and GitHub

## Major Features / Modules

### Module 1: Delegate Management
Manages delegate records throughout the MUN process. Supports adding, searching, updating, deleting, and displaying delegate information using a unique Delegate ID.

### Module 2: CV Information
Accepts delegate CVs and extracts relevant information such as MUN experience and location to support delegate verification and allocation.

### Module 3: Committee & Portfolio Management
Manages committees, available portfolios, committee capacity, and minimum experience requirements.

### Module 4: Delegate Allocation
Supports manual and automatic allocation of delegates to suitable committees and portfolios based on experience, eligibility, availability, and previous allocations.

## System Architecture

The application uses a web-based architecture:

1. **Frontend:** HTML, CSS, and JavaScript provide the user interface.
2. **API Layer:** Node.js and Express.js handle requests through JSON APIs.
3. **Core Processing:** C++ programs perform CV parsing and allocation-related processing.
4. **Database:** MySQL stores delegate, committee, portfolio, and allocation information.

## Project Setup / Installation

### Prerequisites
- Node.js and npm
- MySQL Server
- C++ compiler
- Visual Studio Code
- Git

### Setup Steps

1. Clone the repository:
   ```bash
   git clone <repository-url>
   ```

2. Open the project folder in Visual Studio Code.

3. Install the required Node.js packages:
   ```bash
   npm install
   ```

4. Configure the MySQL database connection using your local database settings.

5. Import the provided SQL database file into MySQL.

6. Ensure the required C++ programs are compiled and configured.

7. Start the application:
   ```bash
   npm start
   ```

8. Open the local URL displayed in the terminal in your browser.

## Current Project Status / Progress

### Working Prototype Developed

The project has progressed from requirement analysis and system design to implementation and integration.

The web application includes delegate management, CV information extraction, committee and portfolio management, and delegate allocation. The frontend communicates with the backend through JSON APIs, while MySQL manages application data and C++ programs handle core processing.

The project is currently available as a local working prototype, with scope for further improvements and deployment.