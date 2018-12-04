open Lwt.Infix;

open Cohttp;

open Geotypes;

[@deriving yojson({strict: false})]
type archive_file = {
  [@key "FileName"]
  name: string,
  [@key "FileSize"]
  size: int,
  [@key "FileUrl"]
  path: string,
  [@key "MimeType"]
  mime_type: string,
  [@key "PageCount"]
  page_count: int,
};

[@deriving yojson({strict: false})]
type archive_document = {
  [@key "Filer"]
  files: list(archive_file),
};

[@deriving yojson({strict: false})]
type archive_item = {
  [@key "Dokumenter"]
  documents: list(archive_document),
};

[@deriving yojson({strict: false})]
type archive_entry = {
  [@key "Items"]
  items: list(archive_item),
};

[@deriving yojson({strict: false})]
type search_response = {
  [@key "Archives"]
  archives: list(archive_entry),
};

let response_to_document_list =
  fun
  | Ok(rsp) =>
    Ok(
      List.fold_left(
        (memo, archive_entry) =>
          List.fold_left(
            (memo, item) =>
              List.fold_left(
                (memo, doc) =>
                  List.fold_left(
                    (memo, file) => {
                      let doc: Document.t = {
                        name: file.name,
                        size: file.size,
                        url: "https://public.filarkiv.dk" ++ file.path,
                        mime_type: file.mime_type,
                        page_count: file.page_count,
                      };
                      [doc, ...memo];
                    },
                    memo,
                    doc.files,
                  ),
                memo,
                item.documents,
              ),
            memo,
            archive_entry.items,
          ),
        [],
        rsp.archives,
      ),
    )
  | Error(_) as err => err;

let search = (~tenant_id, ~id) => {
  let uri =
    Uri.of_string(
      Format.sprintf(
        "https://public.filarkiv.dk/Search?tenantid=%d&adgangsadresseid=%s",
        tenant_id,
        id,
      ),
    );
  Cohttp_lwt_unix.Client.get(uri)
  >>= (
    ((rsp, body)) =>
      Cohttp_lwt.Body.to_string(body)
      >|= (
        body => {
          let code = Response.status(rsp) |> Code.code_of_status;
          if (Code.is_success(code)) {
            switch (Yojson.Safe.from_string(body)) {
            | json =>
              search_response_of_yojson(json) |> response_to_document_list
            | exception (Yojson.Json_error(err)) => Error(err)
            };
          } else {
            Error(Format.sprintf("Filarkiv request failed:\n%s", body));
          };
        }
      )
  );
};