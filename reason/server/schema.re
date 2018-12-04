open Graphql_lwt;

open Geotypes;

let zendesk: Address.t = {
  id: "238947192829",
  tenant_id: 101,
  street: "Snaregade",
  street_number: "12",
  postal_code: "1205",
  city: "K benhavn K",
  floor: None,
  door: None,
  location: {
    lat: 12.57633088,
    lng: 55.67700068,
  },
};

let address =
  Schema.(
    obj("Address", ~fields=_ =>
      [
        field(
          "street", ~typ=non_null(string), ~args=[], ~resolve=((), address) =>
          address.Address.street
        ),
      ]
    )
  );

let schema =
  Schema.(
    schema([
      field("zendesk", ~typ=non_null(address), ~args=[], ~resolve=((), ()) =>
        zendesk
      ),
    ])
  );