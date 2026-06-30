# A url shortener made in C++ w/ Drogon, Postgres, and React)

A complete end to end URL shortener built as a learning project. In progress but the idea is
to learn how to use drogon, use Cpp with Postgres (and hopefully extend into redis) and build 
a sleek frontend with react and TS.

> **What it does:** paste a long url and you get a short one. (might not be short though since im not buying a URL so its going to be a AWS CDN url likely with a shortener sub page, but you get the idea)

> Visiting the short link will redirect to the original link

## Tech Stack

* Frontend: React, Typescript, Vite
* Backend: C++ 20, Drogon
* DB: PostgreSQL
* Local dev: Docker compose
* Deployment: S3/Cloudfront + ECS Fargate mode + RDS with postrgres
* Testing: GoogleTest
* CI/CD: Github Actions (stretch goal)




# url-shortener
