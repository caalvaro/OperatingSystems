let processo1 = {
    pid: 1,
    ppid: 0,
    status: "espera",
    tempoChegada: 0,
    tempoServico: 10,
    tempoQueFoiExecutado: 0,
    tempoCicloAtual: 0,
    io: [
        {tipo: "A", tempo: 3},
        {tipo: "C", tempo: 8}
    ]
};

let processo2 = {
    pid: 2,
    ppid: 0,
    status: "espera",
    tempoChegada: 5,
    tempoServico: 16,
    tempoQueFoiExecutado: 0,
    tempoCicloAtual: 0,
    io: [
        {tipo: "A", tempo: 2},
        {tipo: "B", tempo: 4},
        {tipo: "C", tempo: 7}
    ]
};

let processo3 = {
    pid: 3,
    ppid: 0,
    status: "espera",
    tempoChegada: 7,
    tempoServico: 11,
    tempoQueFoiExecutado: 0,
    tempoCicloAtual: 0,
    io: []
};

let processo4 = {
    pid: 4,
    ppid: 0,
    status: "espera",
    tempoChegada: 7,
    tempoServico: 5,
    tempoQueFoiExecutado: 0,
    tempoCicloAtual: 0,
    io: []
};

let processo5 = {
    pid: 5,
    ppid: 0,
    status: "espera",
    tempoChegada: 11,
    tempoServico: 8,
    tempoQueFoiExecutado: 0,
    tempoCicloAtual: 0,
    io: [
        {tipo: "B", tempo: 5}
    ]
};

let processos = [processo1, processo2, processo3, processo4, processo5];

let filaAltaPrioridade = [];
let filaBaixaPrioridade = [];
let filaIO = [];

let tipoIO = {
    "A": {tempo: 2, prioridadeVolta: "Baixa"},
    "B": {tempo: 4, prioridadeVolta: "Alta"},
    "C": {tempo: 8, prioridadeVolta: "Alta"}
}

let processosTerminados = 0;
let tempo = 0;
let fatiaTempo = 4;
let processoExecutando = null;

// cada iteração é uma unidade de tempo
while (processosTerminados < processos.length) {
    // verifica se um novo processo chegou
    for (let processo of processos) {
        if (processo.tempoChegada == tempo) {
            console.log(`Tempo ${tempo}: processo ${processo.pid} chegou.`);
            filaAltaPrioridade.push(processo);
        }
    }

    // verifica se um processo retornou do IO
    for (let io of filaIO) {
        if (io.tempoSaida == tempo) {
            let prioridade = tipoIO[io.tipo].prioridadeVolta;

            console.log(`Tempo ${tempo}: processo ${io.processo.pid} ficou pronto do IO ${io.tipo}.`);
            
            if (prioridade == "Alta") {
                filaAltaPrioridade.push(io.processo);
            } else {
                filaBaixaPrioridade.push(io.processo);
            }
        }
    }

    // critérios de parada
    if (processoExecutando != null) {
        // verifica quantos processos já terminaram
        if (processoExecutando.tempoQueFoiExecutado == processoExecutando.tempoServico) {
            console.log(`Tempo ${tempo}: processo ${processoExecutando.pid} terminou.`);
            processosTerminados += 1;
            processoExecutando = null;
        }
        // verificando preempção
        else if (processoExecutando.tempoCicloAtual == fatiaTempo) {
            console.log(`Tempo ${tempo}: processo ${processoExecutando.pid} parou a execução -> preempção.`);
            filaBaixaPrioridade.push(processoExecutando);
            processoExecutando.tempoCicloAtual = 0;
            processoExecutando = null;
        }
        // verifica solicitação de IO
        else {
            for (let io of processoExecutando.io) {
                if (io.tempo == processoExecutando.tempoQueFoiExecutado) {
                    console.log(`Tempo ${tempo}: processo ${processoExecutando.pid} parou a execução -> IO ${io.tipo}.`);
                    let tempoIO = tipoIO[io.tipo].tempo;
                    filaIO.push({
                                    processo: processoExecutando,
                                    tempoSaida: tempo + tempoIO,
                                    tipo: io.tipo
                                });
                    
                    processoExecutando.tempoCicloAtual = 0;
                    processoExecutando = null;
                    break;
                }
            }
        }
    }

    // executa um processo se não tiver executando
    if (processoExecutando == null) {
        if (filaAltaPrioridade.length != 0) {
            processoExecutando = filaAltaPrioridade.shift();
        }
        else if (filaBaixaPrioridade.length != 0) {
            processoExecutando = filaBaixaPrioridade.shift();
        }
    }

    // executa o processo
    if (processoExecutando != null) {
        console.log(`Tempo ${tempo}: processo ${processoExecutando.pid} executando.`);
        processoExecutando.tempoQueFoiExecutado += 1;
        processoExecutando.tempoCicloAtual += 1;
    }

    tempo += 1;
}