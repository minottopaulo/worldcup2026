WorldCup 2026 - Sistema de Consulta e Simulação
Sistema em linguagem C para consulta e simulação da Copa do Mundo FIFA 2026 (EUA, Canadá e México), desenvolvido como projeto da disciplina de Algoritmos e Programação 2 do curso de Engenharia da Computação — Uniftec, Caxias do Sul.
Funcionalidades

Consulta de seleções e elencos completos com overall dos atletas
Busca de atletas por nome
Listagem por grupo (A a L)
Simulação da fase de grupos com tabela classificatória
Simulação completa da Copa (oitavas, quartas, semifinais e final)
Carregamento e salvamento de dados via arquivos .txt

Como executar
bash# Gerar os arquivos das 48 seleções (apenas na primeira vez)
gcc gerar_selecoes.c -o gerar_selecoes
./gerar_selecoes

# Compilar e rodar o sistema
gcc wordcup.cpp -o wordcup
./wordcup
Estrutura do projeto
worldcup/
  wordcup.cpp         # sistema principal
  gerar_selecoes.c    # gerador dos elencos
  selecoes/           # 48 arquivos .txt com os elencos oficiais
Tecnologias

Linguagem C
Estruturas, ponteiros, alocação estática, arquivo I/O e ordenação com Bubble Sort

Desenvolvido por Paulo Prado Minotto
