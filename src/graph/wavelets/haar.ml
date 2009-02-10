open Graphics

let foi = float_of_int
let iof = int_of_float


(* UNIQUEMENT POUR LES TESTS
#load "bigarray.cma"
#load "graphics.cma"
;;
open Graphics;;

Graphics.close_graph ();;

let createRep x=
  moveto 50 (50+x);
  lineto 50 50;
  lineto (50+x) 50

let getColor min max coef = 
  let a = max -. min in
  let v = 255 - (iof((coef -. min) *. (255. /. a))) in
    rgb v v v

let getmaxs a =
  let min = ref (infinity) in
  let max = ref (neg_infinity) in    
    for i = 0 to (Bigarray.Array1.dim a) - 1 do
      if (a.{i} < !min) then
	min := a.{i}
      else
	if (a.{i} > !max) then
	  max := a.{i}
	else
	  ();
    done;
    (!min, !max)

let createGraph signal a =
  let size = 300 in
  let len = Bigarray.Array1.dim a in
  let (amin, amax) = (getmaxs a) in
  let nbrRow = iof ((log (foi len)) /. (log 2.)) in
  let sizeRow = (foi size) /. (foi nbrRow) in
    clear_graph ();
    print_float amin;
    print_newline();
    print_float amax;
    createRep size;
    (*Create First Row
    let sizeC = (foi size) /. (foi len) in
      for i = 1 to len do
	draw_rect (iof (50. +. (((foi i) -. 1.) *. sizeC)))
	  (iof (50. +. ((foi nbrRow) -. 1.) *. sizeRow)) 
	  (iof sizeC) (iof sizeRow);	
      done;
     Fin Create First Row *)
      let pos = ref 1 in
      for i = 1 to nbrRow - 1 do
	let nbrC = iof (2. ** (foi (i - 1))) in
	let sizeC = (foi size) /. (foi nbrC) in
	  for j = 1 to nbrC do
	    set_color (getColor amin amax (a.{!pos}));
	    fill_rect (iof(50. +. ((foi j) -. 1.) *. sizeC))
	      (iof (50. +. ((foi i)-.1.) *. sizeRow))
	      (iof sizeC) (iof sizeRow);
	    pos := !pos+1;
	  done;
      done;
      ()
  
let main s =
  let t = haar_direct s in 
    Graphics.open_graph "";
    createGraph (signal 10) t


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
;;

Random.int 256 ;;
let signal x =
  Random.init x ;
  let res = Bigarray.Array1.create (Bigarray.float32)                                                                                                
    Bigarray.c_layout 65536 in
    for i = 0 to 65535 do
      res.{i} <- foi ((Random.int 64000));
    done;
    res
;;

showBig (signal 42)  print_float ;;

let main () =
  (*let signal = echan (f) (0.125) (-.2.) (2.) in*)
  let s = signal 5 in
    showBig s print_float;
    let t = haar_direct s in 
    let a = haar_reverse t in
      showBig (compare a (signal 5)) print_float
;;

main () ;;
filter_directD signal.{0} signal.{1} ;;
 FIN TESTS *)

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
    res

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

(* DESSIN *)

let createRep x=
  moveto 50 (50+x);
  lineto 50 50;
  lineto (50+x) 50

let getColor min max coef = 
  let a = max -. min in
  let v = 255 - (iof((coef -. min) *. (255. /. a))) in
    rgb v v v

let getColor2 min max coef =
  if (coef < 0.) then
    rgb 0 0 0
  else
    if (coef = 0.) then
      rgb 127 127 127
    else
      rgb 255 255 255

let getmaxs a =
  let min = ref (infinity) in
  let max = ref (neg_infinity) in    
    for i = 0 to (Bigarray.Array1.dim a) - 1 do
      if (a.{i} < !min) then
	min := a.{i}
      else
	if (a.{i} > !max) then
	  max := a.{i}
	else
	  ();
    done;
    (!min, !max)

let createGraph signal a =
  let size = 300 in
  let len = Bigarray.Array1.dim a in
  let (amin, amax) = (getmaxs a) in
  let nbrRow = iof ((log (foi len)) /. (log 2.)) in
  let sizeRow = ((foi size) /. (foi nbrRow)) /. 2. in
    clear_graph ();
    print_float amin;
    print_newline();
    print_float amax;
    createRep size;
    (*Create First Row
    let sizeC = (foi size) /. (foi len) in
      for i = 1 to len do
	draw_rect (iof (50. +. (((foi i) -. 1.) *. sizeC)))
	  (iof (50. +. ((foi nbrRow) -. 1.) *. sizeRow)) 
	  (iof sizeC) (iof sizeRow);	
      done;
     Fin Create First Row *)
      let pos = ref 1 in
      for i = 1 to nbrRow - 1 do
	let nbrC = iof (2. ** (foi (i - 1))) in
	let sizeC = (foi size) /. (foi nbrC) in
	  for j = 1 to nbrC do
	    set_color (getColor amin amax (a.{!pos}));
	    fill_rect (iof(50. +. ((foi j) -. 1.) *. sizeC))
	      (iof (50. +. ((foi i)-.1.) *. sizeRow))
	      (iof sizeC) (iof sizeRow);
	    pos := !pos+1;
	  done;
      done;
      ()

let wait x =
  for i = 1 to x do
    let a = (foi i) ** (foi i) /. (foi i) in
      ()
  done;
  ()

let main s =
  let t = haar_direct s in 
    Graphics.open_graph "";
    createGraph t t;
    wait 9999999;
    t

let _ =
  Callback.register "Haar_Direct" main

let _ =
  Callback.register "Haar_Reverse" haar_reverse
