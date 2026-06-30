# Architecture

This doc is for explaining how the pieces fit together and why.

### Whole system in one picture

TODO: Add picture


## Two meaningful operations

1. Shorten (`POST /api/shorten`)

2. Redirect (`GET /{code}`)

## URL Code coming from DB

One trick we can do when creating the code is use the rows auto increment feature.
We can use the unique id from this and we will face no collisions. Then when we shorten it
to base62 it will guarantee distinctness

Custom aliases would take a different path where if we try to insert it into the db, we would hit a 
problem since our DB will have a unique contraint on the redirect names. we would return a `409 conflict`

## Why Drogon

Drogon is incredibly powerful for handling coroutines, also I want to learn it.

We can create co routines to handle io blocking tasks seamlessly with drogon::Task<> and a co_await db call

## Why 302 instead of 301 for redirects

301 is a `Moved Permanently` response, which is good if we want to reduce load on our servers since its cached by the browser.
This is not a problem for us, and more importantly I would like this service to have the ability to get analytics with 302, where
the requests would come back to our server still

## Stateless

There is no memory kept between requests, as should all distributed systems. All state is in postgres. This system will be
horizontally scalable and straightforward to do so in AWS.
