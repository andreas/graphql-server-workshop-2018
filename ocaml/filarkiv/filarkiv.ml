open Lwt.Infix
open Cohttp
open Geotypes

type archive_file = {
  name : string [@key "FileName"];
  size : int [@key "FileSize"];
  path : string [@key "FileUrl"];
  mime_type : string [@key "MimeType"];
  page_count : int [@key "PageCount"];
}
[@@deriving yojson { strict = false }]

type archive_document = {
  files : archive_file list [@key "Filer"];
}
[@@deriving yojson { strict = false }]

type archive_item = {
  documents : archive_document list [@key "Dokumenter"];
}
[@@deriving yojson { strict = false }]

type archive_entry = {
  items : archive_item list [@key "Items"];
}
[@@deriving yojson { strict = false }]

type search_response = {
  archives : archive_entry list [@key "Archives"];
}
[@@deriving yojson { strict = false }]

let response_to_document_list = function
| Ok rsp ->
    Ok (List.fold_left (fun memo archive_entry ->
      List.fold_left (fun memo item ->
        List.fold_left (fun memo doc ->
          List.fold_left (fun memo file ->
            let doc : Document.t = {
              name = file.name;
              size = file.size;
              url = "https://public.filarkiv.dk" ^ file.path;
              mime_type = file.mime_type;
              page_count = file.page_count;
            } in
            doc::memo
          ) memo doc.files
        ) memo item.documents
      ) memo archive_entry.items
    ) [] rsp.archives)
| Error _ as err -> err

let search ~tenant_id ~id =
  let uri = Uri.of_string (Format.sprintf "https://public.filarkiv.dk/Search?tenantid=%d&adgangsadresseid=%s" tenant_id id) in
  Cohttp_lwt_unix.Client.get(uri) >>= fun (rsp, body) ->
  Cohttp_lwt.Body.to_string body >|= fun body ->
  let code = Response.status(rsp) |> Code.code_of_status in
  if Code.is_success code then
    match Yojson.Safe.from_string body with
    | json ->
        search_response_of_yojson json
        |> response_to_document_list
    | exception (Yojson.Json_error err) -> Error err
  else
    Error (Format.sprintf "Filarkiv request failed:\n%s" body)
