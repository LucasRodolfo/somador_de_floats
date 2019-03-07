/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

/*Lucas Rodolfo de Castro Moura RA-156405*/
#include <stdio.h>

/*Verifica se eh um digito numerico ou se eh um ponto(.)*/
int eh_digito(char c)
{
  if ((c >= '0') && (c <= '9') || (c == '.')) return 1;
  else return 0;
}
/*Verifica se eh um caractere espaco*/
int eh_espaco(char c)
{
  if (c == ' ')  return 1;
  else return 0;
}

typedef struct {
  int estado_atual;
} fsm, *Fsm; /* fsm = finite state machine */

/*Funcao que opera a maquina de estados*/
/*Sao usados 3 estados nessa maquina, estado inicial zero que eh usado para detectar o começo e o final das "frases",
o estado 1 que eh usado para detectar quando digitos numericos estao sendo lido,
o estado 2 que mostra quando uma "frase" de caracteres "inuteis" estah sendo linda*/
int opera_fsm(void *this_fsm, char entrada) {
  Fsm maquina = (Fsm) this_fsm;

  switch (maquina->estado_atual) {
    case 0:                             //usado para o inicio das cadeias, marcado pelo espaco em branco
      if (!eh_digito(entrada)) {
        maquina->estado_atual = 2;      //cadeia nao valida de digitos e letras
      }else if (eh_digito(entrada)) {
        maquina->estado_atual = 1;      //cadeia valina de digitos numericos, estado de capturar os digitos
      }
      break;

    case 1:                             //usado para mostrar que essa é uma cadeia valida, de digitos numericos
      if (!eh_digito(entrada) && !eh_espaco(entrada)) {
        maquina->estado_atual = 2;      //cadeia nao valida de digitos e letras, econtrou um caractere nao numerico
      }else if (eh_digito(entrada)) {
        maquina->estado_atual = 1;      //continuou achando um caractere valido, numerico, continua o estado atual
      }else if (eh_espaco(entrada)) {
        maquina->estado_atual = 0;      //comeco de uma nova cadeia a ser avaliada
      }
      break;

    case 2:                             //usado para mostrar que essa é uma cadeia INvalida, de digitos NAO somente numericos
      if (eh_espaco(entrada)) {
        maquina->estado_atual = 0;      //comeco de uma nova cadeia a ser avaliada
      }else{
        maquina->estado_atual = 2;      //cadeia nao valida de digitos e letras, continua no estado
      }
      break;
  }
  return (maquina->estado_atual);
}
/*Funcao que recebe os digitos validos na forma de string e converte eles para inteiros*/
float nmrFloat(char s[]){

  int i=0, cont=0;                      //conta o numero de casas decimais
  float final=0, frac = 0;              //guarda o valor fracionario

  if (s[0] == '.') {                    //Verifica se a cadeira inicia com ponto, se sim, retorna zero, pois a notacao .5 nao eh aceita
    return 0;
  }

  while (s[i] != '\0') {                //caminha pela cadeia numerica e faz as convercoes

    if (s[i] == '.') {                  //se encontrar o ponto(.), calculamos a parte fracionaria
      i++;
      while (s[i] != '\0') {            //caminha na parte fracionaria
        frac = frac * 10;               //converte a parte fracionaria que veio como string em um inteiro
        frac = frac + s[i] - '0';
        cont++;                         //conta a quantidade de casas decimais encontradas apos o ponto(.)
        i++;
        if (s[i] == '.') {              //se outo ponto(.) for encontrado dentro da parte fracionaria, isso eh um notacao invalida
          return 0;
        }
      }
      for (int j = 0; j < cont; j++) {  //converte a parte fracionaria que estah como um inteiro em um float com valores decimais
        frac = frac/10;
      }
      final = final +frac;              //soma a parte decimal com a parte inteira

      return final;
    }else{                              //converte a parte inteira que veio como string para inteiro
      final = final * 10;
      final = final + s[i] - '0';
      i++;
    }
  }

  return final;
}

int main() {

  char buffer_in[200], c, strAux[200];                    //string principal que recebe as cadeias e string auxiliar que guarda somente cadeia numericas
  int pointer=0, iaux=0, laststate=0;                     //laststate guarda o estado da maquina anteriormente
  float total=0;

  for (int i=0; i<200; i++) buffer_in[i] = '\0';          //limpa o vetor principal
  for (int i=0; i<200; i++) strAux[i] = '\0';             //limpa o vetor auxiliar

  do {
    c=getchar();
    buffer_in[pointer++] = c;
  } while (c != '\n');

  fsm maquina;
  maquina.estado_atual = 0;
  pointer = 0;

  while (buffer_in[pointer] != '\n') {                    //caminha na cadeia de caracteres e seta os estados da maquina se acordo com os caracteres

    opera_fsm((void*) (&maquina), buffer_in[pointer]);
    if (maquina.estado_atual == 0){                       //final ou comeco de uma nova parte da cadeia, espaco(' ')
        if (laststate == 1) {                             //se o estado anterior era 1, entao esse eh o fim de uma cadeia de digitos valida
          total = total + nmrFloat(strAux);               //entao, soma o valor dessa ultima cadeia convertida ao valor total da soma
          iaux = 0;                                       //zera o pointer auxiliar da cadeia auxiliar
          for (int i=0; i<200; i++) strAux[i] = '\0';     //limpa vetor auxiliar para ser usado novamente

        }
    }
    if (maquina.estado_atual == 1){                        //se o estado for 1, entao o caracter atual eh um digito numerico
        strAux[iaux] = buffer_in[pointer];                 //entao, coloca mais um caractere na strAux
        iaux++;

    }
    if (maquina.estado_atual == 2) {                        //se o estado for 2, entao o digito atual nao eh um numero
      if (laststate == 1) {                                 //se o estado anterior foi 1 e agora mudou para dois, entao temos uma cadeia mista, nao valida
        for (int i=0; i<200; i++) strAux[i] = '\0';         //limpa a cadeia auxiliar
        iaux = 0;
      }
    }
    laststate = maquina.estado_atual;                       //guarda o estado da maquina
    pointer++;
  }
  if (strAux[0] != '\0') {                                  //se chegamos ao final da cadeia principal e ainda temos digitos validos na cadeia auxiliar
    total = total + nmrFloat(strAux);
    printf("%.10g\n", total);
  }else{
    printf("%.10g\n", total);
  }
  return 0;
}
