# Draw me a database

## Motivation

Challenge project.

Build a useful application for college students using Large Language Models.
The application you build can be of your choosing.
You need to design the best experience possible for them.

You will be judged on your ability to deeply understand and use large language
models, your code abstractions and readability, creating thinking, and being
user driven.

It shouldn't take more than 12 hours.

## Idea

Draw me a database. You are a CS student with a database assignment.

Input the assignment, and ChatGPT helps you draw the conceptual model, which can
then be mechanically converted to a physical model before being exported to a
SQL schema.

Web application with one page of introduction to sell the product on the other
to play with it. Assignment text area, rendered conceptual model graph, the
physical one, and finally the SQL schema.

I had this C++ software "MeriseAcide" I wrote back at school in 2007. It takes a
conceptual database schema as input, written in a simple text file syntax.
Produces a conceptual model graph, mechanically convert it to the physical
model, produce the graph as well, and finally the SQL schema.

After toying with ChatGPT, I feel confident I can prompt it well enough to get
it to convert a database assignment a conceptual model, that can then be fed
into MeriseAcide.

The web stack will be Rust, with the Rocket web framework. The web server will
serve the files (HTML, CSS, JS...) and offer few APIs to forward the query to
ChatGTP, run the MeriseAcide binary, and return the produced files.
The single page client side will be implemented with Vue.js with typescript.
