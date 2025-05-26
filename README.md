# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
<a href="https://www.fiap.com.br/"><img src="assets/logo-fiap.png" alt="FIAP - Faculdade de Informática e Administração Paulista" border="0" width=40% height=40%></a>
</p>

<br>

# FASE 7 / Cap 1 - Consolidação do Sistema FarmTech

## 👨‍🎓 Integrantes: 
- <a href="https://www.linkedin.com/in/a1exlima/">RM559784@fiap.com.br - Alex da Silva Lima</a>
- <a href="https://www.linkedin.com/in/johnatanloriano/">RM559546@fiap.com.br - Johnatan Sousa Macedo Loriano</a>
- <a href="https://www.linkedin.com/in/matheus-maia-655bb1250/">RM560683@fiap.com.br - Matheus Augusto Rodrigues Maia</a>
- <a href="https://www.linkedin.com/in/brunoconter/">RM560518@fiap.com.br - Bruno Henrique Nielsen Conter</a>
- <a href="https://www.linkedin.com/in/fabiosantoscardoso/">RM560479@fiap.com.br - Fabio Santos Cardoso</a>

## 👩‍🏫 Professores:
### Tutor(a) 
- <a href="https://www.linkedin.com/in/leonardoorabona/?originalSubdomain=br">Leonardo Ruiz Orabona</a>
### Coordenador(a)
- <a href="https://www.linkedin.com/in/profandregodoi/">André Godoi</a>

## 📜 Descrição

Este projeto, desenvolvido para a **Fase 7** do curso de Inteligência Artificial da FIAP, consolida os serviços das Fases 1 a 6 da **FarmTech Solutions**, uma empresa fictícia que oferece soluções de IA para o agronegócio. A Fase 7 integra módulos de cálculo de área, manejo de insumos, banco de dados relacional, IoT com ESP32, Machine Learning, visão computacional com YOLO e infraestrutura AWS em uma dashboard interativa. Um serviço de mensageria AWS (SNS/SQS) foi implementado para alertas em tempo real, com todos os serviços acessíveis via comandos no terminal ou botões na interface.

O repositório contém os entregáveis da Fase 7, incluindo a dashboard consolidada, o serviço de mensageria e documentação detalhada, com opção de "ir além" via AWS Rekognition ou algoritmo genético.

## 📂 Estrutura do Repositório

```plaintext
1TIAOR_FIAP_fase7_cap1/
├── .github/               # Configurações do GitHub (workflows, etc.)
├── docs/                  # Documentação e diagramas
├── data/                  # Dados de entrada/saída
├── .gitattributes         # Configurações de atributos do Git
├── .gitignore             # Arquivos e pastas ignorados pelo Git
└── README.md              # Este arquivo
```

## 📦 Entregáveis

### Entrega 1: Dashboard Integrada
- Dashboard Streamlit que unifica os serviços das Fases 1, 2, 3 e 6, com botões para ativação de cada módulo e visualização de dados.
- **Script Principal**: `dashboard/dashboard.py`
- **Vídeo Demonstrativo**: [Link para o vídeo no YouTube](https://www.youtube.com/watch?v=XXXXXXX) (não listado, até 10 minutos)
  - Demonstra a integração de todos os serviços e o funcionamento da dashboard.

### Entrega 2: Serviço de Mensageria AWS
- Implementação de alertas via AWS SNS/SQS, enviando e-mails ou SMS com base em dados de sensores (Fases 1 e 3) e resultados de visão computacional (Fase 6).
- **Script**: `fase5/alert_service.py`
- Inclui prints do console AWS e documentação em `/docs/mensageria.pdf`.

### Entrega 3: "Ir Além" (Opcional)
- **Opção 1: AWS Rekognition**:
  - Tentativa de integração com AWS Rekognition para análise de imagens.
  - **Notebook**: `ir_alem/rekognition.ipynb`
  - Documentação em `/docs/ir_alem_rekognition.md` com prints do console AWS.
  - Vídeo de até 5 minutos (link no README).
- **Opção 2: Algoritmo Genético**:
  - Otimização de insumos com algoritmo genético.
  - **Notebook**: `ir_alem/genetic_algorithm.ipynb`
  - Comparação de desempenho em `/docs/ir_alem_genetic.md`.
  - Vídeo de até 5 minutos (link no README).

## 🔧 Como Utilizar

### Pré-requisitos
- **Python** versão 3.8 ou superior.
- **Bibliotecas**: `streamlit`, `scikit-learn`, `pandas`, `numpy`, `boto3`, `opencv-python`, `yolov5`, `R`.
- **AWS CLI**: Configurado com credenciais do AWS Learner Lab.
- **ESP32**: Configurado com MicroPython ou Arduino IDE (opcional).
- **Jupyter Notebook**: Para executar implementações opcionais.

### Passo a Passo
1. **Clone o repositório**:
   ```bash
   git clone https://github.com/brunocorisco86/1TIAOR_FIAP_fase7_cap1.git
   ```

2. **Navegue até a pasta principal**:
   ```bash
   cd 1TIAOR_FIAP_fase7_cap1
   ```

3. **Instale as dependências**:
   ```bash
   pip install -r requirements.txt
   ```

4. **Execute a dashboard**:
   ```bash
   cd dashboard
   streamlit run dashboard.py
   ```

5. **Assista ao vídeo demonstrativo**:
   - Acesse o link do YouTube fornecido para visualizar a demonstração prática.

## 🗃 Histórico de Lançamentos

* 0.1.0 - 23/05/2025
    * Versão inicial com dashboard, serviço de mensageria e implementações opcionais.

## 📋 Licença

<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1"><p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/"><a property="dct:title" rel="cc:attributionURL" href="https://github.com/brunocorisco86/1TIAOR_FIAP_fase7_cap1">FarmTech Solutions</a> por <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://fiap.com.br">FIAP</a> está licenciado sobre <a href="http://creativecommons.org/licenses/by/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">Attribution 4.0 International</a>.</p>
