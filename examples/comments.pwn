//simple comment #error;
#a;

/* another simple comment #error;*/
#b;

/* multi
   line
   comment
   #error; should not be in the ast*/

/* nested #error; /* comments #error;
  */ with multiple
  
  #error;
  lines*/ #c;
