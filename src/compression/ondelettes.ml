let foi = float_of_int
let iof = int_of_float

let echan signal p bInf bSup =
  let nbrPoints = iof ((bSup -. bInf)/.p) in
  let res = Array.create nbrPoints 0. in
  let x = ref bInf in
    for i=0 to nbrPoints - 1 do
      res.(i) <- signal !x;
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
    Array.map f signal

let reapprox signal longueur=
  let coef = (foi (longueur))**(1./.2.) in
  let f x = coef *. x in
    Array.map f signal

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

(* Je me demande VRAIMENT ce que ce truc fout ici ...
 *
 * 3 / 2 ;;
*)

(* Formule specifique a cette ondelette *)
let next2 f longueur t = 
  let l2 = longueur / 2 in
  let res = Array.create l2 0.  in
    for p = 0 to l2 - 1 do
      for i=(2*p) to (2*p+1) do
        res.(p)<-res.(p) +. (f (i-2*p) *. t.(i))
      done;
    done; 
    res

(* on monte d'un cran *)
let prec2 g h d a longueur = 
  let res = Array.create (longueur * 2) 0. in
    for p = 0 to (longueur * 2) - 1 do
      res.(p) <- a.(p / 2) +. (((-.1.)**(foi p)) *. d.(p / 2))
    done; 
    res

(* On descend tout ! *)
let calculCoefs h l signal=
  let n = iof((log (foi l)) /. (log 2.)) in
  let g = g h in

  let l2 = ref l in    
  let ds = Array.create n [||] in
  let a = ref (approx signal l) in
    for i=0 to n-1 do
      ds.(i) <- (next2 g !l2 !a);
      a := next2 h !l2 !a;
      l2 := !l2 / 2
    done;
    (!a, ds)

(*
let retrouveSignal h l (a, d) =
  let g = g h in
  let a = Array.create in
  let ds = ref l in
    for i=0 to l - 1 do
      a := (prec h g (List.hd !ds) !a);
      ds := List.tl !ds
    done;
    reapprox !a
;;
 *)

let ondelette = 
  let signal = echan (sin) (0.015625) (-.512.) (512.) in
  let a = calculCoefs h 65536 signal in
(*     print_string "FINI\n"; *)
    a

let _ = 
  Callback.register "ondelettes algo" ondelette

