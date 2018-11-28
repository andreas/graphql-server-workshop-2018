# Workshop on GraphQL servers in ReasonML/OCaml

This material is intended for the [ReasonML CPH meetup Nov 28th 2018](https://www.meetup.com/ReasonML-CPH/events/256170465/).

## Setup (esy)

- `yarn global add esy@next / npm install -g esy@next`
- `esy`

## Run (esy)

- `esy x server`

## Setup (Trusty ol' way)

- OCaml 4.07.1, or ReasonML 3.0.4
- OPAM 2.0.1
- OPAM packages: dune, graphql-lwt, px_deriving_yojson

Installation instructions for OCaml: https://medium.com/@bobbypriambodo/getting-your-feet-wet-with-ocaml-ea1045b6efbc

Installation instructions for Reason: https://reasonml.github.io/docs/en/installation

## Run (Trusty ol' way)

- `dune exec src/server/server.exe`

---

We recommend installing [VSCode](https://code.visualstudio.com/) and a suitable extension for OCaml/ReasonML.

## Resources

- [OCaml cheat sheet](http://www.ocamlpro.com/files/ocaml-lang.pdf)
- [Standard library cheat sheet](http://www.ocamlpro.com/files/ocaml-stdlib.pdf)
- [Reason Syntax cheat sheet](https://reasonml.github.io/docs/en/syntax-cheatsheet)
- [`ocaml-graphql-server` documentation](https://andreas.github.io/ocaml-graphql-server/graphql-lwt/Graphql_lwt/Schema/index.html)
- [`ocaml-graphql-server` examples](https://github.com/andreas/ocaml-graphql-server#examples)
- [Sketch.sh (Online REPL)](https://sketch.sh/)
