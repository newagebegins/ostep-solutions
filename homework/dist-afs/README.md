# Chapter 50: The Andrew File System (AFS)

## 2

> What is the precise condition for one to take place?

Callback occurs when a client has a file in the cache, and this file was modified on the server by some other client.

## 4

> Of all the possible schedule interleavings of the two clients’ operations, how many of them lead to client 1 reading the value 1, and how many reading the value 0?

Client 1 will read the value 1 only if it opens the file after client 0 has already closed the file.

## 6

> What is important in determining the final value of the file?

Last writer wins.
