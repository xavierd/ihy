let foi = float_of_int
let iof = int_of_float

type float_array =
    (float, Bigarray.float32_elt, Bigarray.c_layout) Bigarray.Array1.t


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

let removeFirsts (t : float_array) pow = 
  let len = Bigarray.Array1.dim t in
    for i = len / pow to len - 1 do
      t.{i} <- 0.;
    done;
    ()
;;

let removeSeuil (t : float_array) cut s =
  let len = Bigarray.Array1.dim t in
(*  let nbrRow = iof ((log (foi len)) /. (log 2.)) in*)
    for i = 16 to len/cut - 1 do
(*      let echelle = (foi nbrRow) -. ((log (foi i)) /. (log 2.)) in*)
	if (abs_float(t.{i}) <= s) then
	  begin
	    t.{i} <- 0.;
	  end
	else
	  ()
    done;
    ()

let countZero (t : float_array) =
  let len = Bigarray.Array1.dim t in
  let count = ref 0 in
    for i=0 to len - 1 do
      if (t.{i} = 0.) then
	count := !count + 1
      else
	()
    done;
    !count
      
let egalize (t : float_array) cut s =
  let len = Bigarray.Array1.dim t in
  let mark = Array.make len 0 in
(*  let a = ref 0. in
    for i=16 to len/cut-1 do
      a := t.{i}
    done;
    !a
*)
    for i=16 to len/cut-1 do
      if (mark.(i) = 0) then
	begin
	  mark.(i)<-1;
	  let aux x = 
	    let a = abs_float(x -. t.{i}) in
	      ((a <= s) && (x!=0.))
	  in
	  let count = ref 1 in
	  let sum = ref t.{i} in 
	    for j=i+1 to len/cut-1 do
	      if ((mark.(j)=0) && (aux t.{j})) then
		begin
		  count := !count +1;
		  sum := !sum +. t.{j};
		  mark.(j)<-1
		end
	      else
		()
	    done;
	    let middle = !sum /. (foi !count) in
	      for j=i+1 to len/cut-1 do
		if (mark.(j) = 1) then
		  begin
		    t.{j} <- middle;
		    mark.(j) <- 2;
		  end
		else
		  ()
	      done
	end
      else
	()
    done
  
let compress (t : float_array) =
  removeFirsts t 2;
  (*removeSeuil t 1 250.;*)
  egalize t 2 30.;
  (*removeSeuil t 2 500.;*)
  t      
;;

;;
(*
  Valeurs normales et valeurs de bords pour Daubechies avec p=2 
  VOIR PAGE 286 du livre de stephane mallat pour valeurs et
  explications 
*)
let coefs = function
    -1 -> 0.482962913145
  | 0 ->  0.836516303738
  | 1 ->  0.224143868042
  | 2 -> -.0.129409522551
  | _ -> failwith "Coefficients error"

let coefsR = function
    -1 -> -.0.129409522551
  | 0 ->  -.0.224143868042
  | 1 ->  0.836516303738
  | 2 -> -.0.482962913145
  | _ -> failwith "Coefficients error"

let leftLCoefs = function
    (0, 0) -> 0.6033325119
  | (0, 1) -> 0.690895531
  | (1, 0) -> 0.03751746045
  | (1, 1) -> 0.4573276599
  | _ -> failwith "Coefficients error"
     
let leftMCoefs = function
    (0, 2) -> -0.398312997
  | (1, 2) -> 0.227428117
  | (1, 3) -> 0.2238203570
  | (1, 4) -> 0.1292227434
  | _ -> failwith "Coefficients error"
     
let rightLCoefs = function
    (-2, -2) -> 0.1901514184
  | (-2, -1) -> -.0.1942334074
  | (-1, -2) -> 0.434896998
  | (-2, -1) -> 0.8705087534
  | _ -> failwith "Coefficients error"

let rightMCoefs = function
    (-2, -5) -> 0.4431490496
  | (-2, -4) -> 0.7675566693
  | (-2, -3) -> 0.3749553316
  | (-1, -3) -> 0.2303890438
  | _ -> failwith "Coefficients error"
    
(* Fin des Valeurs *)

let daubechies_direct (a : float_array) =
  let p = 2 in
  let len = Bigarray.Array1.dim a in
  let nbrRow = iof ((log (foi len)) /. (log 2.)) in
  let res = Bigarray.Array1.create (Bigarray.float32)
    Bigarray.c_layout len in
  let a2 = Bigarray.Array1.create (Bigarray.float32)
    Bigarray.c_layout len in
  let value = ref 0. in
  let curLen = ref (len / 2) in
    for i=1 to nbrRow do
      let (prev, next) =
	if (i mod 2 = 1) then
	  (a, a2)
	else
	  (a2, a)
      in
	(* LEFT SIDE *)
	for k=0 to (p-1) do
	  value := 0;
 	  for l=0 to (p-1) do
	    value := !value +. (leftLCoefs (k,l)) *. prev.{l}
	  done;
	  for m=p to (p+2*k) do
	    value := !value +. (leftMCoefs (k,m)) *. prev.{m}
	  done;
	  next.{k} <- !value
	done;
	(* MIDDLE *)
	for k=p to !curLen - p - 1 do
	  value := 0;
	  for l=(-p+1) to p do
	    value := !value +. (coefs l) *. prev.{2*k+p}
	  done;
	  next.{k} <- !value
	done;
	(* RIGHT SIDE *)
	for k=-p to -1 do
	  value := 0;
          for l=-p to -1 do
            value := !value +. (rightLCoefs (k,l)) *. prev.{2*!curLen+l}
          done;
          for m=-p+2*k+1 to (-p-1) do
            value := !value +. (rightMCoefs (k,m)) *. prev.{2*!curLen+m}
          done;
          next.{!curLen + k} <- !value
	done;
	    
    done

;;
let _ =
  Callback.register "Daubechies_Direct" daubechies_direct

let _ =
  Callback.register "Daubechies_Reverse" daubechies_reverse
