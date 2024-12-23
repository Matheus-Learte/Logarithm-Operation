# Pgm-s-image-logarithm-Operation
Processador de imagem que se utiliza de uma operação através de logaritmo para melhorar fotos .pgm com intensidades muito dinâmicas dos pixels.
## Operação
y=c*log(1+x), tal que c é uma constante que se é calculada através da equação c=255/log(1+max), onde max é o pixel com o maior valor. A variável x representa cada um dos pixels da foto e y é o pixel depois de ser processado.
