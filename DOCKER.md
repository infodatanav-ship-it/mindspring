# Dockerizing this app

Quick instructions to build and run the app using Docker and MariaDB.

1) Build and start services:

```bash
docker-compose up --build -d
```

This will build the `web` image (PHP 7.3.3 + Apache) and start a MariaDB 10.3.39 container.

2) Access the app:
- Open http://localhost:8080 in your browser.

3) Database credentials (defaults in `docker-compose.yml`):
- root: `rootpass`
- database: `mindspring`
- user: `mindspring` / password: `password`

4) Notes:
- If you need MariaDB tag `10.3.39.1` change the `image` for `db` in `docker-compose.yml` accordingly. If that exact tag is unavailable, use `mariadb:10.3.39`.
- The app files are mounted into the container so edits on the host are reflected immediately.
- If you need Composer, you can `docker exec -it $(docker-compose ps -q web) bash` and install or run Composer inside the container.
