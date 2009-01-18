let foi = float_of_int
let iof = int_of_float ;;

let showBig array f len =
  for i=0 to len - 1 do
    print_int i ;
    print_string " -> ";
    f array.{i};
    print_newline () ;
  done

let bigMap f a =
  for i=0 to (Bigarray.Array1.dim a) - 1 do
    a.{i} <- f a.{i}
  done;
  a

let initBigArray array len value = 
  for i=0 to len - 1 do
    array.{i} <- value
  done
;;

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

(* filtres mirroir conjugue *)
let h x = if (x=0 or x=1) then (2. ** (-.(1.)/.(2.))) else 0.
let g h n = ((-.1.)**(1.-. (foi(n)))) *. h(1-n) 

(* premiere (re)approximation de je sais plus quoi *)
let approx signal longueur=
  let coef = (foi(longueur))**(-.1./.2.) in
  let f x = coef *. x in
    bigMap f signal

let reapprox signal longueur=
  let coef = (foi (longueur))**(1./.2.) in
  let f x = coef *. x in
    bigMap f signal

(* on descend d'un cran ! *)
(* Formule general *)
let next f longueur t = 
  let l2 = longueur / 2 in
  let res = Array.create l2 0.  in
    for p = 0 to l2 - 1 do
      for i=0 to longueur - 1 do
        res.(p)<-res.(p) +. (f (i-2*p) *. t.(i))
      done;
    done; 
    res

(* Formule specifique a cette ondelette *)
let next2 f longueur readtab curPosRead writetab curPosWrite = 
  let l2 = longueur in
    for p = 0 to l2 - 1 do
      for i=(2*p) to (2*p+1) do
        writetab.{p+curPosWrite}<-writetab.{p+curPosWrite} +. (f (i-2*p) *. readtab.{i+curPosRead})
      done;
    done; 
    ()

(* on monte d'un cran *)
let prec2 g h d a longueur =
  let res = Bigarray.Array1.create (Bigarray.float32)
    Bigarray.c_layout (longueur * 2) in
    for p = 0 to (longueur * 2) - 1 do
      res.{p} <- a.{p / 2} +. (((-.1.)**(foi p)) *. d.{p / 2})
    done; 
    res

let calculCoefs h l signal =
  let g = g h in
  let res = Bigarray.Array1.create (Bigarray.float32)
    Bigarray.c_layout l in
  let a = (approx signal l) in
  let an = (Bigarray.Array1.create (Bigarray.float32) Bigarray.c_layout (l/2)) in
  let l2 = ref l in
  let curPosWrite = ref l in
  let n = iof((log (foi l)) /. (log 2.)) in
    initBigArray res l 0. ;
    initBigArray an (l/2) 0.;
    for i=0 to n-1 do
      curPosWrite := !curPosWrite - (!curPosWrite / 2);
      l2 := !l2 / 2;
      if (i mod 2 = 0) then
	begin
	  next2 g !l2 a 0 res !curPosWrite;
	  next2 h !l2 a 0 an 0; 
	end
      else
	begin
	  next2 g !l2 an 0 res !curPosWrite;
          next2 h !l2 an 0 a 0;
	end; 
    done;
    res.{0} <- a.{0};
    res

let ondelette = 
  let signal = echan (sin) (0.125) (-2.) (2.) in
  let a = calculCoefs h 32 signal in
    (*     print_string "FINI\n"; *)
    (*    showBig a print_float 32; *)
    a



let _ = 
  Callback.register "ondelettes algo" ondelette

