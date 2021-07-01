Bit Parser (Simple XML & HTML Parser)
======
The purpose of this project is to read the necessary information and remove unnecessary parts, rather than completely parsing HTML files.

The process of generating a DOM tree itself causes significant overhead. Elements of XML and HTML documents are a tree, thus I assume information extraction and removing some parts can be expressed as simple elements tree transversing model. So we will use simple finite state machines to generate transversing events.
