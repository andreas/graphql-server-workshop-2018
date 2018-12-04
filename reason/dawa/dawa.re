open Cohttp;

open Lwt.Infix;

[@deriving yojson({strict: false})]
type kommune = {kode: string};

[@deriving to_yojson]
type koordinater = {
  lat: float,
  lng: float
};

let koordinater_of_yojson =
  fun
  | `List([`Float(lat), `Float(lng)]) => Ok({lat, lng})
  | _ => Error("Invalid location");

[@deriving yojson({strict: false})]
type adgangspunkt = {koordinater};

[@deriving yojson({strict: false})]
type vejstykke = {adresseringsnavn: string};

[@deriving yojson({strict: false})]
type postnummer = {
  nr: string,
  navn: string
};

[@deriving yojson({strict: false})]
type adgangsadresse = {
  id: string,
  adgangspunkt,
  kommune,
  vejstykke,
  husnr: string,
  postnummer
};

[@deriving yojson({strict: false})]
type item = {
  id: string,
  adgangsadresse,
  etage: option(string),
  [@key "dør"] doer: option(string)
};

let items_to_addresses =
  fun
  | Ok(addresses) =>
    Ok(
      List.map(
        (item) => (
          {
            id: item.adgangsadresse.id,
            tenant_id: int_of_string(item.adgangsadresse.kommune.kode),
            street: item.adgangsadresse.vejstykke.adresseringsnavn,
            street_number: item.adgangsadresse.husnr,
            postal_code: item.adgangsadresse.postnummer.nr,
            city: item.adgangsadresse.postnummer.navn,
            floor: item.etage,
            door: item.doer,
            location: {
              lat: item.adgangsadresse.adgangspunkt.koordinater.lat,
              lng: item.adgangsadresse.adgangspunkt.koordinater.lng
            }
          }: Geotypes.Address.t
        ),
        addresses 
      )
    )
  | Error(_) as err => err;

let search = (query) => {
  let uri = Uri.of_string(Format.sprintf("https://dawa.aws.dk/adresser?q=%s", query));
  Cohttp_lwt_unix.Client.get(uri)
  >>= (
    ((rsp, body)) =>
      Cohttp_lwt.Body.to_string(body)
      >|= (
        (body) => {
          let code = Response.status(rsp) |> Code.code_of_status;
          if (Code.is_success(code)) {
            switch (Yojson.Safe.from_string(body)) {
            | json => items_to_addresses([%of_yojson : list(item)](json))
            | exception (Yojson.Json_error(err)) => Error(err)
            };
          } else {
            Error(Format.sprintf("DAWA request failed:\n%s", body));
          };
        }
      )
  );
};