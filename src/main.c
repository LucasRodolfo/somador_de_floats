/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar. 
 */

/*Lucas Rodolfo de Castro Moura RA-156405*/
#include <stdio.h>

int eh_digito(char c)
{
  if ((c >= '0') && (c <= '9') || (c == '.')) return 1;
  else return 0;
}

int eh_espaco(char c)
{
  if (c == ' ')  return 1;
  else return 0;
}

typedef struct {
  int estado_atual;
} fsm, *Fsm; /* fsm = finite state machine */

int opera_fsm(void *this_fsm, char entrada) {
  Fsm maquina = (Fsm) this_fsm;

  switch (maquina->estado_atual) {
    case 0:
      if (!eh_digito(entrada)) {
        maquina->estado_atual = 2; //cadeia nao valida de digitos e letras
      }else if (eh_digito(entrada)) {
        maquina->estado_atual = 1; //estado de contar os digitos
      }
      break;

    case 1:
      if (!eh_digito(entrada) && !eh_espaco(entrada)) {
        maquina->estado_atual = 2; //cadeia nao valida de digitos e letras
      }else if (eh_digito(entrada)) {
        maquina->estado_atual = 1; //estado de contar os digitos
      }else if (eh_espaco(entrada)) {
        maquina->estado_atual = 0; //comeco de uma nova cadeia a ser avaliada
      }
      break;

    case 2:
      if (eh_espaco(entrada)) {
        maquina->estado_atual = 0; //comeco de uma nova cadeia a ser avaliada
      }else{
        maquina->estado_atual = 2; //cadeia nao valida de digitos e letras
      }
      break;
  }
  return (maquina->estado_atual);
}

float nmrFloat(char s[]){

  int i=0, cont=0/*conta o numero de casas decimais*/;
  float final=0, frac = 0;

  if (s[0] == '.') {
    return 0;
  }

  while (s[i] != '\0') {

    if (s[i] == '.') {
      i++;
      while (s[i] != '\0') {
        frac = frac * 10;
        frac = frac + s[i] - '0';
        cont++;
        i++;
        if (s[i] == '.') {
          return 0;
        }
      }
      for (int j = 0; j < cont; j++) {
        frac = frac/10;
      }

      final = final +frac;

      return final; // ou return aux + frac
    }else{
      final = final * 10;
      final = final + s[i] - '0';
      i++;
    }
  }

  return final;
}

int main() {

  char buffer_in[200], c, strAux[200];
  int pointer=0, iaux=0, laststate=0;
  float total=0;

  for (int i=0; i<200; i++) buffer_in[i] = '\0';
  for (int i=0; i<200; i++) strAux[i] = '\0';

  do {
    c=getchar();
    buffer_in[pointer++] = c;
  } while (c != '\n');

  fsm maquina;
  maquina.estado_atual = 0;
  pointer = 0;

  while (buffer_in[pointer] != '\n') {

    opera_fsm((void*) (&maquina), buffer_in[pointer]);
    if (maquina.estado_atual == 0){
        if (laststate == 1) {
          total = total + nmrFloat(strAux);
          iaux = 0;
          for (int i=0; i<200; i++) strAux[i] = '\0';
          /* converte strAux para int e soma no total e iaux=0*/
        }
    }
    if (maquina.estado_atual == 1){
        strAux[iaux] = buffer_in[pointer];
        iaux++;
        //coloca mais um caractere na strAux
    }
    if (maquina.estado_atual == 2) {
      if (laststate == 1) {
        for (int i=0; i<200; i++) strAux[i] = '\0';
        iaux = 0;
        /* converte strAux para int e soma no total e iaux=0*/
      }
    }
    laststate = maquina.estado_atual;
    pointer++;
  }
  if (strAux[0] != '\0') {
    total = total + nmrFloat(strAux);
    printf("%.10g\n", total);
  }else{
    printf("%.10g\n", total);
  }
  return 0;
}
