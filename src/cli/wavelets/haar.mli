val foi : int -> float
val iof : float -> int
val filter_direct : float -> float -> float
val filter_directD : float -> float -> float
val haar_direct :
  (float, 'a, 'b) Bigarray.Array1.t ->
  (float, Bigarray.float32_elt, Bigarray.c_layout) Bigarray.Array1.t
