let () =
  Graphql_lwt.Server.start ~ctx:(fun _req -> ()) Schema.schema
  |> Lwt_main.run
