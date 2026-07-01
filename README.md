# Projeto Teoria dos Grafos

Este projeto consiste em uma biblioteca em Linguagem C para manipulação, representação e análise de grafos de grande porte. A aplicação foi desenvolvida estruturando um Tipo Abstrato de Dados (TAD) capaz de alternar dinamicamente entre as representações de **Matriz de Adjacência** e **Lista de Adjacência**, permitindo avaliar os impactos práticos de desempenho, consumo de memória e tempo de execução.

## 🚀 Funcionalidades

A biblioteca possui implementações eficientes para:
* **Leitura de Grafos:** Carga de dados a partir de arquivos de texto formatados.
* **Busca em Largura (BFS):** Geração de árvore geradora induzida, registrando pai e nível de cada vértice.
* **Busca em Profundidade (DFS):** Varredura vertical completa registrando caminhos de recursão.
* **Cálculo de Distância Mínima:** Retorna o menor caminho (quantidade de arestas) entre dois nós.
* **Determinação de Diâmetro:** Identificação da maior distância periférica da estrutura.
* **Componentes Conexas:** Mapeamento, contagem e identificação de subgrafos desconexos.
* **Exportação de Estatísticas:** Geração automática de relatórios textuais de métricas.

## 📊 Dados de Teste (Arquivos de Grafos)
Por motivos de boas práticas e limitações de tamanho de arquivo no GitHub, a pasta grafos/ contendo as instâncias de teste brutas (ex: grafo_1.txt) não foi incluída neste repositório.

## 🛠️ Como Compilar e Executar
1. Certifique-se de ter o compilador GCC instalado na sua máquina.
2. Crie uma pasta chamada saidas/ no diretório raiz para que o programa exporte os resultados: `mkdir -p saidas`
3. Compile os arquivos de código-fonte:
`gcc main.c grafo.c -o graph_analyzer`
4. Execute o programa gerado: `./graph_analyzer`
   
Crie uma pasta chamada saidas/ no diretório raiz para que o programa exporte os resultados:

## 📁 Estrutura do Projeto

```text
├── main.c              # Fluxo principal de testes e medição de desempenho
├── grafo.c             # Implementação das funções e algoritmos de busca
├── grafo.h             # Definição do TAD e assinaturas das funções
├── .gitignore          # Proteção para não enviar arquivos pesados ao GitHub
└── README.md           # Documentação do projeto
