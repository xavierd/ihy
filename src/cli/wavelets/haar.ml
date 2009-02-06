let foi = float_of_int
let iof = int_of_float

(*
(* UNIQUEMENT POUR LES TESTS *)


let showBig a f =
  for i=0 to (Bigarray.Array1.dim a) - 1 do
    print_int i ;
    print_string " -> ";
    f a.{i};
    print_newline ();
  done

let echan signal p bInf bSup =
  let nbrPoints = iof ((bSup -. bInf)/.p) in
  let res = Bigarray.Array1.create (Bigarray.float32)
    Bigarray.c_layout nbrPoints in
  let x = ref bInf in
    for i=0 to nbrPoints - 1 do
      res.{i} <- signal !x;
      x := !x +. p
    done;
    res

let signal = echan (sin) (0.125) (-.1.) (1.)

let main () =
  let signal = echan (sin) (0.125) (-.1.) (1.) in
    showBig signal print_float;
    showBig (haar_direct signal) print_float
;;

main () ;;      

filter_directD signal.{0} signal.{1} ;;
(* FIN TESTS *)
*)

let filter_direct x y =
  let coef = (2. ** (-.(1.)/.(2.))) in
    coef *. (x +. y)
    
let filter_directD x y =
  let coef = (2. ** (-.(1.)/.(2.))) in
    coef *. (y -. x)

let haar_direct a =
  let len = Bigarray.Array1.dim a in
  let nbrRow = iof ((log (foi len)) /. (log 2.)) in
  let res = Bigarray.Array1.create (Bigarray.float32)
    Bigarray.c_layout len in
  let posInit = ref 0 in
  let posRes = ref (len - 1) in
    for i=1 to nbrRow do
      let stepVal = (iof (2. ** (foi (i-1)))) in
      let stepInit = stepVal * 2 in
	posInit := len - stepInit;
	for j=1 to (len / stepInit) do
	  res.{!posRes} <- (filter_directD a.{!posInit} a.{!posInit + stepVal});
	  a.{!posInit} <- (filter_direct a.{!posInit} a.{!posInit +
							  stepVal});
	  posRes := !posRes - 1;
	  posInit := !posInit - stepInit;
	done;
    done;
    res.{0} <- a.{0};
    res

let _ =
  Callback.register "Haar_Direct" haar_direct
