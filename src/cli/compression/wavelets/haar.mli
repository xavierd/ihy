val foi : int -> float
val iof : float -> int
type float_array =
    (float, Bigarray.float32_elt, Bigarray.c_layout) Bigarray.Array1.t
val removeFirsts : float_array -> int -> unit
val removeSeuil : float_array -> int -> float -> unit
val countZero : float_array -> int
val compress : float_array -> float_array
val filter_direct : float -> float -> float
val filter_directD : float -> float -> float
val filter_reverse : float -> float -> (float -> float -> float) -> float
val haar_direct : float_array -> float_array
val haar_reverse :
  float_array ->
  float_array
