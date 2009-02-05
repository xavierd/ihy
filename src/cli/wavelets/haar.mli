val foi : int -> float
val iof : float -> int
val showBig : ('a, 'b, 'c) Bigarray.Array1.t -> ('a -> 'd) -> int -> unit
val bigMap :
  ('a -> 'a) ->
  ('a, 'b, 'c) Bigarray.Array1.t -> ('a, 'b, 'c) Bigarray.Array1.t
val initBigArray : ('a, 'b, 'c) Bigarray.Array1.t -> int -> 'a -> unit
val echan :
  (float -> float) ->
  float ->
  float ->
  float -> (float, Bigarray.float32_elt, Bigarray.c_layout) Bigarray.Array1.t
val h : int -> float
val g : (int -> float) -> int -> float
val approx :
  (float, 'a, 'b) Bigarray.Array1.t ->
  int -> (float, 'a, 'b) Bigarray.Array1.t
val reapprox :
  (float, 'a, 'b) Bigarray.Array1.t ->
  int -> (float, 'a, 'b) Bigarray.Array1.t
val next : (int -> float) -> int -> float array -> float array
val next2 :
  (int -> float) ->
  int ->
  (float, 'a, 'b) Bigarray.Array1.t ->
  int -> (float, 'c, 'd) Bigarray.Array1.t -> int -> unit
val prec2 :
  'a ->
  'b ->
  (float, 'c, 'd) Bigarray.Array1.t ->
  (float, 'e, 'f) Bigarray.Array1.t ->
  int -> (float, Bigarray.float32_elt, Bigarray.c_layout) Bigarray.Array1.t
val calculCoefs :
  (int -> float) ->
  int ->
  (float, 'a, 'b) Bigarray.Array1.t ->
  (float, Bigarray.float32_elt, Bigarray.c_layout) Bigarray.Array1.t
val ondelette :
  (float, 'a, 'b) Bigarray.Array1.t ->
  (float, Bigarray.float32_elt, Bigarray.c_layout) Bigarray.Array1.t
