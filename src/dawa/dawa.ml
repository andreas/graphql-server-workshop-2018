open Cohttp
open Lwt.Infix

type kommune = {
  kode : string;
}
[@@deriving yojson { strict = false }]

type koordinater = {
  lat : float;
  lng : float;
}
[@@deriving to_yojson]
let koordinater_of_yojson = function
  | `List [`Float lat; `Float lng] -> Ok { lat; lng }
  | _ -> Error "Invalid location"

type adgangspunkt = {
  koordinater : koordinater;
}
[@@deriving yojson { strict = false }]

type vejstykke = {
  adresseringsnavn : string;
}
[@@deriving yojson { strict = false }]

type postnummer = {
  nr : string;
  navn : string;
}
[@@deriving yojson { strict = false }]

type adgangsadresse = {
  id : string;
  adgangspunkt : adgangspunkt;
  kommune : kommune;
  vejstykke : vejstykke;
  husnr : string;
  postnummer : postnummer;
}
[@@deriving yojson { strict = false }]

type item = {
  id : string;
  adgangsadresse : adgangsadresse;
  etage : string option;
  doer : string option [@key "dør"];
}
[@@deriving yojson { strict = false }]

let items_to_addresses = function
| Ok addresses ->
  Ok (List.map (fun item ->
    ({
      id = item.adgangsadresse.id;
      tenant_id = int_of_string item.adgangsadresse.kommune.kode;
      street = item.adgangsadresse.vejstykke.adresseringsnavn;
      street_number = item.adgangsadresse.husnr;
      postal_code = item.adgangsadresse.postnummer.nr;
      city = item.adgangsadresse.postnummer.navn;
      floor = item.etage;
      door = item.doer;
      location = {
        lat = item.adgangsadresse.adgangspunkt.koordinater.lat;
        lng = item.adgangsadresse.adgangspunkt.koordinater.lng;
      }
    } : Geotypes.Address.t)
  ) addresses)
| Error _ as err -> err

let search query =
  let uri = Uri.of_string (Format.sprintf "https://dawa.aws.dk/adresser?q=%s" query)  in
  Cohttp_lwt_unix.Client.get(uri) >>= fun (rsp, body) ->
  Cohttp_lwt.Body.to_string body >|= fun body ->
  let code = Response.status(rsp) |> Code.code_of_status in
  if Code.is_success code then
    match Yojson.Safe.from_string body with
    | json -> items_to_addresses ([%of_yojson: item list] json)
    | exception (Yojson.Json_error err) -> Error err
  else
    Error (Format.sprintf "DAWA request failed:\n%s" body)
