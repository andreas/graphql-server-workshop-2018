type location = {
  lat : float;
  lng : float;
}

type t = {
  id : string;
  tenant_id : int;
  street : string;
  street_number : string;
  postal_code : string;
  city : string;
  floor : string option;
  door : string option;
  location : Location.t;
}
