// Captura os elementos HTML da página
const textoGpsEsquerdaLatitude = document.getElementById("gps-esquerda-latitude");
const textoGpsEsquerdaLongitude = document.getElementById("gps-esquerda-longitude");
const botaoAtualizaPosicoes = document.getElementById("botao-atualiza-posicoes");

// Função que inicializa o WebSocket (WS)
function inicializar() {
    const url = "ws://192.168.4.1:1337/";
    wsConnect(url);
}

function wsConnect(url) {
    websocket = new WebSocket(url);

    websocket.onopen = (evento) => {abreConexao(evento)};
    websocket.onclose = (evento) => {fechaConexao(evento)};
    websocket.onmessage = (evento) => {recebeMensagem(evento)};
    websocket.onerror = (evento) => {recebeErro(evento)};
}

function abreConexao(evento) {
    // Printa no console que cliente foi conectado
    console.log('Conectado ao WS do Atlas');

    // Faz as requisições das informações necessárias
    enviaMensagem("getGpsEsquerdaLatitudeAtual");
    enviaMensagem("getGpsEsquerdaLongitudeAtual");

    // Libera o botão de atualizar as posições
    botaoAtualizaPosicoes.disabled = false;
}

function fechaConexao(evento) {
    // Printa no console que o cliente foi desconectado
    console.log('Desconectado');

    // Trava o botão de atualizar as posições
    botaoAtualizaPosicoes.disabled = true;

    // Tenta reconectar depois de 2 segundos
    setTimeout(() => wsConnect(url), 2000);
}

// Função executada ao receber uma mensagem do WS
function recebeMensagem(evento) {
    console.log("Received: " + evento.data);

    // Aqui podemos tratar essa mensagem como quisermos
    switch(evento.data) {
        case "0":
            break;
        case "1":
            break;
        default:
            break;
    }
}

// Recebe um erro do WS e printa no console do navegador
function recebeErro(evento) {
    console.log("ERROR: " + evento.data);
}

// Envia uma mensagem para o WS
function enviaMensagem(mensagem) {
    console.log("Enviando: " + mensagem);
    websocket.send(mensagem);
}

// Executada quando o botão HTML é pressionado (exemplo)
function atualizarPosicoes() {
    enviaMensagem("getGpsEsquerdaLatitudeAtual");
    enviaMensagem("getGpsEsquerdaLongitudeAtual");
}

window.addEventListener("load", inicializar, false);