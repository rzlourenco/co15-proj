// external builtin functions
import #argc()
import $argv(#n)
import #atoi($s)

// external user functions
import #factorial(#n)

// the main function
#pwn() {
  #f = 1;
  "Teste para a função factorial"!!

  if (argc() == 2)
    f = atoi(argv(1));

  f! "! = "! factorial(f)!!
}

