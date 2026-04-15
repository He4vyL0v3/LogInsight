<div align="center">

# OWL API 🍀

<img src="doc/owl.png" width="90%" />

[![Backend CI Pipeline](https://github.com/Nighty3098/owl_backend/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/Nighty3098/owl_backend/actions/workflows/ci.yml)

</div>

---

## Overview

**OWL_BACKEND** is a microservice-based RESTful API for project, board, and task management, featuring user authentication, subscription management, and admin monitoring. All services are built with Flask, use PostgreSQL, and are orchestrated via Docker Compose.

### Microservices

- **Auth Service** (`:5001`): User registration, authentication, JWT issuing
- **Users Service** (`:5002`): User profile management
- **Core Service** (`:5000`): Projects, boards, columns, tasks (CRUD, reordering, color, etc.)
- **Subscription Service** (`:5004`): Subscription plans and user subscriptions
- **Backup Service** (`:5005`): Database backup (internal)

---

## Features

- JWT authentication (10-year token lifetime)
- Role-based access (USER, ADMIN)
- Rate limiting and CORS support
- Comprehensive logging and error handling
- Admin endpoints for monitoring and user management
- Docker Compose orchestration

---

# OWL Project Architecture

```mermaid
graph TB
    subgraph Frontend["Frontend (Electron)"]
        E[Electron App<br>React + TypeScript]
        E -->|HTTPS/WSS| Vercel[Vercel Hosting<br>owl-gamma.vercel.app]
    end

    subgraph Backend["Backend Services (Docker)"]
        subgraph API["API Services (Flask)"]
            API_Gateway[API Gateway<br>nginx/reverse proxy]
            Core[Core Service<br>:5000<br>Projects, Boards,<br>Tasks, Columns]
            Auth[Auth Service<br>:5001<br>Login, Register,<br>Token validation]
            Users[Users Service<br>:5002<br>Profile, Settings,<br>Delete account]
            Subs[Subscription Service<br>:5003<br>Plans, Billing,<br>Subscriptions]
        end

        subgraph Data["Data Layer"]
            PG[(PostgreSQL<br>:5432)]
            Redis[(Redis<br>:6379)]
        end

        Backup[Backup Service<br>:5004<br>DB Backups]
    end

    E -->|POST /api/v1/login| Auth
    E -->|POST /api/v1/register| Auth
    E -->|POST /api/v1/check_auth| Auth

    E -->|GET /api/v1/get_boards<br>POST /api/v1/create_task<br>POST /api/v1/edit_task| Core
    E -->|GET /api/v1/get_projects<br>POST /api/v1/save_project| Core

    E -->|POST /api/v1/change_password<br>POST /api/v1/change_username<br>POST /api/v1/delete| Users
    E -->|GET /api/v1/get_public_users| Users

    E -->|GET /api/v1/subs<br>GET /api/v1/user_subscription_info| Subs

    Auth -->|Validate Token| PG
    Core -->|Cache/Rate Limit| Redis
    Core -->|CRUD| PG
    Users -->|CRUD| PG
    Subs -->|CRUD| PG
    Backup -->|Backup| PG

    API_Gateway -->|Route| Auth
    API_Gateway -->|Route| Core
    API_Gateway -->|Route| Users
    API_Gateway -->|Route| Subs
```

## Components Description

| Service | Port | Technology | Purpose |
|---------|------|-------------|---------|
| **Core Service** | 5000 | Flask + SQLAlchemy | Main API: Projects, Boards, Tasks, Columns, Settings |
| **Auth Service** | 5001 | Flask | Authentication: Login, Register, Token generation, Ban check |
| **Users Service** | 5002 | Flask | User management: Profile, Password, Email, Username |
| **Subscription Service** | 5003 | Flask | Subscriptions: Plans, User subscriptions, Admin controls |
| **Backup Service** | 5004 | Python | Database backups automation |
| **PostgreSQL** | 5432 | PostgreSQL 16 | Primary database |
| **Redis** | 6379 | Redis | Caching, Rate limiting |

## API Endpoints Overview

### Auth Service (`/api/v1/`)
- `POST /register` - User registration
- `POST /login` - User login
- `GET /check_auth` - Token validation

### Core Service (`/api/v1/`)
- `GET /get_boards` - Get user's boards
- `POST /create_board` - Create board
- `POST /create_task` - Create task
- `POST /edit_task` - Edit task
- `POST /delete_board` - Delete board
- `GET /get_projects` - Get projects
- `POST /save_project` - Save project

### Users Service (`/api/v1/`)
- `POST /change_password` - Change password
- `POST /change_username` - Change username
- `POST /change_email` - Change email
- `POST /delete` - Delete account

### Subscription Service (`/api/v1/`)
- `GET /subs` - Get all subscription plans
- `GET /user_subscription_info` - Get user's subscription
- `POST /change_user_subscription` - Admin: change user plan

---

## Quick Start

### 1. Clone the Repository

```bash
git clone https://github.com/Nighty3098/OWL_BACKEND
cd OWL_BACKEND
```

### 2. Configure Environment Variables

Create a `.env` file in the project root with the following variables:

```ini
SECRET_KEY=...
POSTGRES_ROOT_PASSWORD=...
POSTGRES_USER=...
POSTGRES_PASSWORD=...
POSTGRES_DATABASE=...
FLASK_ENV=production
SERVICE_PORT=5000
SERVICE_NAME=core_service
PG_PORT=5433
DATABASE_URL=postgresql://${POSTGRES_USER}:${POSTGRES_PASSWORD}@db:${PG_PORT}/${POSTGRES_DATABASE}
AUTH_SERVICE_PORT=5001
AUTH_SERVICE_NAME=auth_service
SUBSCRIPTION_SERVICE_PORT=5004
SUBSCRIPTION_SERVICE_NAME=subscription_service
USERS_SERVICE_PORT=5002
USERS_SERVICE_NAME=users_service
BACKUPS_SERVICE_NAME=backup_service
BACKUPS_SERVICE_PORT=5005
```

### 3. (Optional) Generate SSL Certificates (Development Only)

```bash
mkdir -p certs && certs
mkcert -install
mkcert localhost 127.0.0.1 ::1
```

```bash
openssl req -x509 -newkey rsa:4096 -sha256 -days 3650 -nodes \
-keyout key.pem -out cert.pem -subj "/CN=YOURIP" \
-addext "subjectAltName=IP:YOURIP"
```

### 4. Database Migration and Table Creation

The project uses Alembic for database migrations. Tables are automatically created during the first startup:

```bash
# Build and start all services (includes automatic migration)
docker-compose build
docker-compose up

# Or manually run migrations if needed:
docker-compose run --rm migrate flask --app manage.py db init
docker-compose run --rm migrate flask --app manage.py db migrate -m "Initial tables"
docker-compose run --rm migrate flask --app manage.py db upgrade
```

> **Important:** at the first launch, Docker executes scripts from `db_init_scripts'. They create both a `postgres` system user and a user from the `POSTGRES_USER` variable, so the errors `role "postgres" does not exist` and `password authentication failed for user "<app_user>"`no longer block the download.

**Expected Tables:**
- `user` - User accounts and authentication
- `plan` - Subscription plans
- `subscription` - User subscriptions
- `project` - User projects
- `board` - Project boards
- `column` - Board columns
- `task` - Column tasks
- `user_login` - Login history
- `user_settings` - User preferences

### 5. Verify Database Setup

Check that all tables were created successfully:

```bash
docker-compose exec db psql -U $POSTGRES_USER -d $POSTGRES_DATABASE -c "\dt"
```

To stop the services:

```bash
docker-compose down
```

---

## Architecture

Each microservice runs independently and communicates via REST. All services use Flask + Gunicorn, connect to PostgreSQL, and are managed with Docker Compose. The database is shared via a single PostgreSQL container.

- **API Gateway:** Each service exposes its own port and API endpoints
- **Stateless:** All authentication is JWT-based
- **Admin endpoints:** For monitoring, banning users, and database stats

---

## Main Endpoints (Core Service)

- `/api/v1/get_projects` — Get all projects for the user
- `/api/v1/save_projects` — Create or update a project
- `/api/v1/delete_project` — Delete a project
- `/api/v1/get_boards` — Get all boards for the user
- `/api/v1/create_board` — Create a new board
- `/api/v1/create_column` — Create a new column
- `/api/v1/create_task` — Create a new task
- `/api/v1/delete_board` — Delete a board
- `/api/v1/rename_board` — Rename a board
- `/api/v1/rename_column` — Rename a column
- `/api/v1/delete_column` — Delete a column
- `/api/v1/delete_task` — Delete a task
- `/api/v1/edit_task` — Edit a task
- `/api/v1/set_task_completed` — Mark a task as completed
- `/api/v1/set_column_color` — Set color for a column
- `/api/v1/set_task_color` — Set color for a task
- `/api/v1/change_tasks_column` — Move task to another column
- `/api/v1/reorder_board` — Reorder boards
- `/api/v1/reorder_column` — Reorder columns
- `/api/v1/reorder_task` — Reorder tasks
- `/api/v1/reorder_projects` — Reorder projects
- `/api/users` — List all users (admin)
- `/api/ban_user` — Ban/unban user (admin)
- `/api/database/stats` — Database stats (admin)
- `/api/database/queries` — Query stats (admin)
- `/api/usage` — Server usage info (admin)
- `/api/version` — API version
- `/api/health` — Health check

> **See [doc/API.md](./doc/API.md) for full endpoint documentation and request/response examples.**

---

## Security

- JWT authentication (store tokens securely on the client)
- Roles: USER, ADMIN (admin endpoints: `/api/usage`, `/api/users`, `/api/ban_user`, `/api/v1/change_user_subscription`)
- Rate limiting per endpoint
- CORS and input validation
- Logging of all requests and errors

---

## Error Handling

Standard HTTP status codes and error messages:

- **400 Bad Request:** Invalid request or missing fields
- **401 Unauthorized:** Invalid credentials or token
- **403 Forbidden:** Insufficient permissions
- **404 Not Found:** Resource not found
- **500 Internal Server Error:** Server-side error

---

## Troubleshooting

### Database Tables Not Created?

1. **Check migration status:**
   ```bash
   docker-compose exec api flask --app manage.py db current
   ```

2. **Manual migration:**
   ```bash
   docker-compose run --rm migrate flask --app manage.py db stamp head
   docker-compose run --rm migrate flask --app manage.py db upgrade
   ```

3. **Force table creation:**
   ```bash
   docker-compose exec api python -c "
   from manage import app, db
   with app.app_context():
       db.create_all()
       print('Tables created successfully')
   "
   ```

---

## Useful Links

- [Full API Documentation](./doc/API.md)
- [OWL Web Application](https://owl-gamma.vercel.app/)

---

**Built with OWL API 🦉**
