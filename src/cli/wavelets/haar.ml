let foi = float_of_int
let iof = int_of_float

(*
(* UNIQUEMENT POUR LES TESTS *)
#load "bigarray.cma"

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

let compare t1 t2 =
  let res = Bigarray.Array1.create (Bigarray.float32)
    Bigarray.c_layout (Bigarray.Array1.dim t1) in
    for i = 0 to (Bigarray.Array1.dim t1) - 1 do
      res.{i} <- t1.{i} -. t2.{i};
    done;
    res

let main () =
  let signal = echan (sin) (0.125) (-.2.) (2.) in
    showBig signal print_float;
    let t = haar_direct signal in
    let a = haar_reverse t in
      showBig (compare signal a) print_float
;;

main () ;;
filter_directD signal.{0} signal.{1} ;;
(* FIN TESTS *)
*)

let removeFirsts t = 
  let len = Bigarray.Array1.dim t in
    for i = len / 4 to len - 1 do
      t.{i} <- 0.;
    done;
    ()
;;
abs 
;;

let removeSeuil t s =
  let len = Bigarray.Array1.dim t in
  let nbrRow = iof ((log (foi len)) /. (log 2.)) in
    for i = 1 to len - 1 do
      let echelle = (foi nbrRow) -. ((log (foi i)) /. (log 2.)) in
	if (abs_float(t.{i} *. (echelle ** 1.5)) <= s) then
	  begin
	    t.{i} <- 0.;
	  end
	else
	  ()
    done;
    ()

let countZero t = 
  let len = Bigarray.Array1.dim t in
  let count = ref 0 in
    for i=0 to len - 1 do
      if (t.{i} = 0.) then
	count := !count + 1
      else
	()
    done;
    !count
      
  
let compress t =
  (*  removeFirsts t; *)
  removeSeuil t 3000.;
  let count = countZero t in
    print_int count;
    print_newline ();
    print_int (iof ((foi count) /. (foi (Bigarray.Array1.dim t)) *. 100.)); 
    print_newline ();
    print_newline ();
    t      

let filter_direct x y =
  let coef = (2. ** (-.(1.)/.(2.))) in
    coef *. (x +. y)

let filter_directD x y =
  let coef = (2. ** (-.(1.)/.(2.))) in
    coef *. (y -. x)

let filter_reverse y x op=
  let coef = (2. ** (-.(1.)/.(2.))) *. 2.  in
    (op y x) /. coef


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
	  a.{!posInit} <- (filter_direct a.{!posInit} a.{!posInit + stepVal});
	  posRes := !posRes - 1;
	  posInit := !posInit - stepInit;
	done;
    done;
    res.{0} <- a.{0};
    compress res

let haar_reverse a =
  let len = Bigarray.Array1.dim a in
  let nbrRow = iof ((log (foi len)) /. (log 2.)) in
  let posInit = ref 0 in
  let t1 = Bigarray.Array1.create (Bigarray.float32)
    Bigarray.c_layout len in
  let t2 = Bigarray.Array1.create (Bigarray.float32)
    Bigarray.c_layout len in
    t1.{0} <- a.{0};
    for i=1 to nbrRow do
      let (prev, next) =
	if (i mod 2 = 1) then
	  (t1, t2)
	else
	  (t2, t1)
      in
      let step = (iof (2. ** (foi (i-1)))) in
	posInit := step;
	let j = ref 0 in
	  while (!j < (step * 2)) do
	    next.{!j} <- filter_reverse (prev.{!j/2}) (a.{!posInit}) (-.);
	    next.{!j + 1} <- filter_reverse (prev.{!j/2}) (a.{!posInit}) (+.);
	    j := !j + 2;
	    posInit := !posInit + 1;
	  done;
   done;
    if (nbrRow mod 2 = 1) then
      t2
    else
      t1



let _ =
  Callback.register "Haar_Direct" haar_direct

let _ =
  Callback.register "Haar_Reverse" haar_reverse
