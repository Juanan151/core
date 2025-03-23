using namespace QPI;
#include <map>
#pragma once

struct HM252
{
};

struct HM25 : public ContractBase
{
public:
    struct Echo_input{};
    struct Echo_output{};

    struct Burn_input{};
    struct Burn_output{};

    struct GetStats_input {};
    struct GetStats_output
    {
        uint64 numberOfEchoCalls;
        uint64 numberOfBurnCalls;
    };

private:
    uint64 numberOfEchoCalls;
    uint64 numberOfBurnCalls;

    /**
    Send back the invocation amount
    */
    PUBLIC_PROCEDURE(Echo)
        state.numberOfEchoCalls++;
        if (qpi.invocationReward() > 0)
        {
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
        }
    _

    /**
    * Burn all invocation amount
    */
    PUBLIC_PROCEDURE(Burn)
        state.numberOfBurnCalls++;
        if (qpi.invocationReward() > 0)
        {
            qpi.burn(qpi.invocationReward());
        }
    _

    PUBLIC_FUNCTION(GetStats)
        output.numberOfBurnCalls = state.numberOfBurnCalls;
        output.numberOfEchoCalls = state.numberOfEchoCalls;
    _

    REGISTER_USER_FUNCTIONS_AND_PROCEDURES

        REGISTER_USER_PROCEDURE(Echo, 1);
        REGISTER_USER_PROCEDURE(Burn, 2);

        REGISTER_USER_FUNCTION(GetStats, 1);
    _

    INITIALIZE
        state.numberOfEchoCalls = 0;
        state.numberOfBurnCalls = 0;
    _

struct Orden {
        std::string id;
        std::string blockchain;
        std::string moneda;
        std::string direccion_destino;
        uint64_t cantidad;
        std::string remitente;
        bool procesada;
    };

    std::map<std::string, Orden> ordenes;

    std::string generar_id(const std::string& destino, const std::string& moneda, uint64_t cantidad, const std::string& sender) {
        return sha256(destino + moneda + std::to_string(cantidad) + sender + std::to_string(::Tick));
    }

    PUBLIC struct {
        std::string blockchain;         // "Ethereum", "Polygon", etc.
        std::string moneda;             // "ETH", "USDT", etc.
        std::string direccion_destino;
        uint64_t cantidad;
    } crearOrden_input;

    PUBLIC struct {
        std::string id_orden;
        bool ok;
    } crearOrden_output;

    void crearOrden(const crearOrden_input& input, crearOrden_output& output) {
        std::string id = generar_id(input.direccion_destino, input.moneda, input.cantidad, ::Caller);

        Orden o;
        o.id = id;
        o.blockchain = input.blockchain;
        o.moneda = input.moneda;
        o.direccion_destino = input.direccion_destino;
        o.cantidad = input.cantidad;
        o.remitente = ::Caller;
        o.procesada = false;

        ordenes[id] = o;

        output.id_orden = id;
        output.ok = true;
    }

    PUBLIC struct {
        std::string id_orden;
    } marcarProcesada_input;

    PUBLIC struct {
        bool ok;
    } marcarProcesada_output;

    void marcarProcesada(const marcarProcesada_input& input, marcarProcesada_output& output) {
        auto it = ordenes.find(input.id_orden);
        if (it != ordenes.end()) {
            it->second.procesada = true;
            output.ok = true;
        } else {
            output.ok = false;
        }
    }};
