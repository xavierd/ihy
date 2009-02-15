val foi : int -> float
val iof : float -> int
type array_type =
    (float, Bigarray.float32_elt, Bigarray.c_layout) Bigarray.Array1.t
val removeFirsts : array_type -> unit
val removeSeuil : array_type -> float -> unit
val countZero : array_type -> int
val compress : array_type -> array_type
val filter_direct : float -> float -> float
val filter_directD : float -> float -> float
val filter_reverse : float -> float -> (float -> float -> float) -> float
val haar_direct : array_type -> array_type
val haar_reverse :
  array_type ->
  array_type
