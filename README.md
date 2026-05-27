# RSA Project (Matematica Discreta)

Projeto academico de criptografia RSA com interface web em Django e a logica criptografica implementada em C.

## Stack

- Python 3
- Django 4.1
- HTML + CSS com templates do Django
- C para a camada RSA
- SQLite, que e o banco padrao do Django neste projeto

## Arquitetura

### Frontend

O frontend fica nos templates em `projetomd/mdproject/templates/mdproject/`.

As telas principais sao:

- `index.html`: menu inicial
- `save.html`: formulario para gerar e salvar chave publica
- `crypt.html`: formulario para criptografar uma mensagem
- `decrypt.html`: formulario para descriptografar uma mensagem

O layout visual e controlado por `projetomd/mdproject/static/css/styles.css`.

### Backend

O backend e um app Django chamado `mdproject`.

As rotas principais sao:

- `/mdproject/`
- `/mdproject/save/`
- `/mdproject/crypt/`
- `/mdproject/descrypt/`

As views recebem os dados dos formularios, convertem os campos para inteiro quando necessario e chamam funcoes da biblioteca C via `ctypes`.

O banco SQLite existe na estrutura do projeto, mas nao ha models com persistencia real sendo usados no fluxo atual.

### Camada C

O arquivo C central para a versao web e `projetomd/mdproject/c_code/menu.c`.

Ele concentra as funcoes para:

- validar primos e calcular `mdc`
- gerar chave publica
- criptografar
- descriptografar
- gravar resultados em arquivos texto

Os arquivos de saida usados pelo fluxo sao:

- `chavePub.txt`
- `textEncript.txt`
- `textDencript.txt`

## Arquivos C necessarios

Para o programa web funcionar, o essencial e:

- `projetomd/mdproject/c_code/menu.c`
- a biblioteca compilada a partir dele, como `menu.dll` no Windows ou `menu.so` no Linux

Os demais arquivos C do repositório nao sao necessarios para o fluxo web atual:

- `encriptar.c`
- `desencriptar.c`
- `menu.c` na raiz do projeto
- `publicKeyTests/public.c`

Esses arquivos sao versoes CLI, testes ou prototipos do mesmo algoritmo.

## Como rodar o projeto

### 1. Criar o ambiente virtual

No linux:
```bash
python3 -m venv .venv
source .venv/bin/activate
```
No windows:
**criar**
```bash
python -m venv .venv
```
**cmd**
```bash
.\.venv\Scripts\activate.bat
```
**PowerShell**
```bash
.\.venv\Scripts\Activate.ps1
```
### 2. Instalar as dependencias

```bash
pip install -r requirements.txt
```
**para iniciar o servidor pule para o passo 5

### 3. Enviar o zip com os arquivos C

Na tela inicial (`/mdproject/`), envie um arquivo `.zip` contendo **obrigatoriamente**:

- `encriptar.c`
- `desencriptar.c`

O sistema vai descompactar, substituir os arquivos atuais e compilar automaticamente os executaveis usando `gcc`.

### 4. Ajustar o caminho da biblioteca no backend

O arquivo `projetomd/mdproject/views.py` carrega a biblioteca C por meio de `ctypes.CDLL(...)`.

No Linux, ele agora procura automaticamente por `menu.so` dentro de `projetomd/mdproject/c_code/`.

### 5. Preparar o banco e iniciar o servidor

```bash
python manage.py migrate
python manage.py runserver
```

Depois abra:

```bash
http://127.0.0.1:8000/mdproject/
```

## Para generalizar para alunos

Se a ideia e deixar o aluno focar apenas na logica do RSA, o melhor caminho e manter o Django somente como interface e pedir que o aluno mexa apenas em `projetomd/mdproject/c_code/menu.c`.

Se quiser simplificar ainda mais, o mesmo RSA pode ser separado em um projeto C puro, deixando o frontend/backend apenas como uma camada opcional.

## Observacoes

- O projeto foi escrito com uma dependencia de biblioteca C carregada por `ctypes`, entao o caminho da DLL ou da SO precisa bater com o sistema operacional.
- O nome da rota de descriptografia esta como `descrypt` no codigo atual.
