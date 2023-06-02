Se você é um entusiasta de jardinagem ou agricultura, provavelmente já se deparou com o desafio de entender as necessidades exatas das suas plantas. Afinal, saber a umidade do solo, a umidade do ar e a temperatura pode ser crucial para garantir um ambiente saudável e propício ao crescimento. Por isso nosso projeto propõe desenvolver um painel capaz de mostrar a umidade do solo e de quebra mostrar a temperatura e umidade do ar. Utilizando uma placa ESP32 Wroom customizada pelo pessoal da DIY More (que possui um sensor de umidade do solo capacitivo juntamente com um DHT11) conseguimos desenvolver um projeto que possa ficar "espetado" em qualquer vazo ou local de sua casa que possua terra, isso pelo fato dele vir equipado com um sistema de carregamento e um suporte para baterias de 3.7v (Li-ION do tipo 18650).

Para construir o projeto foi utilizado a seguinte infra-estrutura (mas você pode optar por utilizar os componentes separados):

	- 1x Placa ESP32 Wroom DIY More (ou esp32 dev kit comum)
	- 1x Sensor de umidade do solo capacitivo
	- 1x Sensor de temperatura e umidade do ar (DHT11)
	- 1x Bateria 18650 / Ou qualquer fonte de alimentação que forneça 3.7v
	- 1x Suporte para bateria 18650 (se for utilizar a bateria)
	- 1x Cabo micro USB (para gravar o firmware e se nessário alimentar a placa)
	- 1x Protoboard (opcional)
	- 1x Caixa para armazenar o projeto (opcional)
	- Cobertura WiFi
	- Uma conexão com a internet
	- Um computador com o Arduino IDE instalado
	- Uma instalação do Node-RED
	- Uma instalação do PostgreSQL
	- Um broker MQTT configurado, no nosso caso, utilizamos um broker público fornecido pelo pessoal da EMQX (https://www.emqx.io/)

Como estamos utilizando a placa ESP32 Wroom DIY More, não é necessário utilizar nenhum tipo de resistor ou capacitor para
utilizar o sensor de umidade do solo, pois a placa já vem com um circuito de condicionamento de sinal para o sensor.

Configuração da infraestrutura

No nosso projeto utilizamos docker para configurar o Node-RED e o PostgreSQL, mas você pode utilizar qualquer outra forma de instalação. O docker é uma das melhores alternativas, visto que a instalação é simples e rápida, além de ser possível utilizar o docker-compose para configurar os containers. Para instalar o docker, basta seguir o tutorial oficial: https://docs.docker.com/engine/install/

Observação: Há um arquivo do docker-compose na raíz do projeto!

Após a instalação do docker, basta acessar o local onde o projeto foi clonado e executar o seguinte comando para instalar o Node-RED e o PostgreSQL:

```bash
$ docker-compose up -d
```

Com isso o Node-RED estará disponível para acesso na porta 1880 e o PostgreSQL na porta 5432.

Ao clonar o repositório, tanto o Node-RED quanto o PostgreSQL já estarão configurados, porém, é necessário criar o banco de dados e a tabela que será utilizada para armazenar os dados. Para isso, basta acessar o Node-RED utilizando o usuário "admin" e a senha "Ifsp2023", e importar o arquivo de fluxo que está na raíz do projeto (flows.json). Para que o projeto inicie, basta executar o bloco "prepare" no fluxo chamado "Preparação da base de dados". Com isso, o banco de dados e a tabela serão criados.

Configuração do firmware

No firmware disponível no diretório "esp" dentro do arquivo "esp.ino", é necessário alterar as seguintes linhas:

```c
const char* ssid = "";
const char* password = "";
const char* mqttTopic = "";
```

Nessas variáveis você deve definir os valores que se encaixam no que você deseja. O ssid e a senha são referentes a sua rede WiFi, e o mqttTopic é o tópico que será utilizado para publicar as mensagens MQTT. No nosso caso, utilizamos um tópico que consiste em um UUID versão 4, mas você pode utilizar qualquer outro tópico que desejar. O nosso tópico se parece com isso: "b6f0a0a0-1b1b-4b1b-8b1b-1b1b1b1b1b1b".

Após alterar essas variáveis, basta gravar o firmware na placa ESP32 Wroom DIY More utilizando o Arduino IDE.

Com isso o projeto estará pronto para ser utilizado.

Como funciona?

Em resumo, o nosso ESP32 coleta os dados do DHT11 e do sensor de umidade do solo, e publica esses dados no tópico MQTT "/input-data". O Node-RED, que está configurado para se inscrever nesse tópico, recebe esses dados e os armazena no banco de dados.

Com os dados armazenados, o Node-RED disponibiliza a informação em tempo real através de um dashboard, que pode ser acessado através do endereço ENDERECO-IP:1880/ui. Onde ENDERECO-IP é o endereço IP da máquina onde o Node-RED está instalado.

Conclusão

Com isso, através do uso desse dispositivo, somos capacitados a entender melhor as necessidades específicas das nossas plantas, adaptando o ambiente de acordo com as condições ideais para seu desenvolvimento saudável, podendo até otimizar o uso da água, evitando tanto a escassez quanto o excesso, e tomar medidas preventivas contra problemas relacionados à umidade e temperatura. Há também espaço para melhorias, como um controle de irrigação automático, que pode ser feito através de um relé que ativa uma solenóide, liberando a água para irrigação e permitindo monitorar e controlar através do painel de controle.

Muito obrigado pela atenção e espero que tenham gostado do projeto!

Escrito por Luiz Antonio F. Gomes e Yago Bomfim