# Workshop on GraphQL servers in ReasonML/OCaml

This material is intended for the [ReasonML CPH meetup Nov 28th 2018](https://www.meetup.com/ReasonML-CPH/events/256170465/).

## Reason

Installation instructions for Reason: https://reasonml.github.io/docs/en/installation

### Setup


Delete the folder `ocaml` and rename the folder `reason` to `src`:

```
rm -r ocaml && mv reason src
```

Then install dependencies:

```
yarn global add esy@next / npm install -g esy@next
esy
```

### Run

```
esy x server
```

## OCaml

Installation instructions for OCaml: https://medium.com/@bobbypriambodo/getting-your-feet-wet-with-ocaml-ea1045b6efbc

### Setup

Delete the folder `reason` and rename the folder `ocaml` to `src`:

```
rm -r reason && mv ocaml src
```

Then install dependencies:

```
opam install dune graphql-lwt ppx_deriving_yojson
```

### Run

```
dune exec src/server/server.exe
```

## Editor

We recommend installing [VSCode](https://code.visualstudio.com/) and a suitable extension for OCaml/ReasonML.



## Slides

- [OCaml slides](https://andreas.github.io/graphql-server-workshop-2018/slides)
- [Reason slides](https://andreas.github.io/graphql-server-workshop-2018/slides/index.reason.html)

## Resources

- [OCaml cheat sheet](http://www.ocamlpro.com/files/ocaml-lang.pdf)
- [Standard library cheat sheet](http://www.ocamlpro.com/files/ocaml-stdlib.pdf)
- [Reason Syntax cheat sheet](https://reasonml.github.io/docs/en/syntax-cheatsheet)
- [`ocaml-graphql-server` documentation](https://andreas.github.io/ocaml-graphql-server/graphql-lwt/Graphql_lwt/Schema/index.html)
- [`ocaml-graphql-server` examples](https://github.com/andreas/ocaml-graphql-server#examples)
- [Sketch.sh (Online REPL)](https://sketch.sh/)
