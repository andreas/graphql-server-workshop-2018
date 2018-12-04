let () =
  Graphql_lwt.Server.start(~ctx=_req => (), Schema.schema) |> Lwt_main.run;