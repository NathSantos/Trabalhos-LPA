// ======================================== DÉCIMO TRABALHO ========================================
#include <stdio.h> 
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>

typedef struct {
    uint8_t daddr[6]; // Endereco MAC de destino
    uint8_t saddr[6]; // Endereco MAC de origem (source)
    uint16_t protocol; // Protocolo da camada superior (IP!)
} ethernet_hdr_t;

typedef struct {
    uint8_t hdr_len:4, // Tamanho do Cabeçalho,
    version:4; // Versão do IP
    uint8_t tos; // Tipo de serviço
    uint16_t tot_len; // Tamanho total do IP
    uint16_t id; // Id do pacote
    uint16_t frag_off; // Fragmentado?
    uint8_t ttl; // Tempo de vida
    uint8_t protocol; // Protocolo da camada superior (TCP!)
    uint16_t check; // Checksum
    uint8_t saddr[4]; // Endereço IP de origem
    uint8_t daddr[4]; // Endereço IP de destino
} ip_hdr_t;

typedef struct {
    uint16_t sport; // Porta TCP de origem
    uint16_t dport; // Porta TCP de destino
    uint32_t seq; // Sequência
    uint32_t ack_seq; // Acknowledgement
    uint8_t reservado:4, // Não usado
    hdr_len:4; // Tamanho do cabecalho
    uint8_t flags; // Flags do TCP
    uint16_t window; // Tamanho da janela
    uint16_t check; // Checksum
    uint16_t urg_ptr; // Urgente
} tcp_hdr_t;


// =================================================================================================


void main(int argc, char *argv[]) {

    FILE *file = fopen(argv[1], "rb");
    ethernet_hdr_t struct_ethernet;
    ip_hdr_t struct_ip;
    tcp_hdr_t struct_tcp;

    printf("Lendo Ethernet ..\n");   
    fread(&struct_ethernet, sizeof(ethernet_hdr_t), 1, file);
    printf("    --> MAC de Origem: ");
    for(int i=0; i<6; i++) {
         if(i == 5) {
            printf("%.2x", struct_ethernet.saddr[i]);
         } else{
            printf("%.2x:", struct_ethernet.saddr[i]);
         }
    }
    printf("\n    --> MAC de Destino: ");
    for(int i=0; i<6; i++) {
        if(i == 5) {
            printf("%.2x", struct_ethernet.daddr[i]);
        } else {
            printf("%.2x:", struct_ethernet.daddr[i]);
        }
    }
    printf("\n");

    printf("Lendo IP ..\n");
    fread(&struct_ip, sizeof(ip_hdr_t), 1, file);
    printf("    --> Versão do IP: %d\n", struct_ip.version);
    printf("    --> Tamanho do cabeçalho: %d bytes\n", (struct_ip.hdr_len * 4));
    printf("    --> Tamanho do pacote: %d bytes\n", ntohs(struct_ip.tot_len));
    printf("    --> Endereço IP de Origem: ");
    for(int i=0; i<4; i++) {
        if(i == 3) {
            printf("%.2d", struct_ip.saddr[i]);
        } else {
            printf("%.2d.", struct_ip.saddr[i]);
        }
    }
    printf("\n    --> Endereço IP de Destino: ");
    for(int i=0; i<4; i++) {
        if(i == 3) {
            printf("%d", struct_ip.daddr[i]);
        } else {
            printf("%d.", struct_ip.daddr[i]);
        }
    }
    printf("\n");

    fseek(file, (struct_ip.hdr_len * 4) - sizeof(ip_hdr_t), SEEK_CUR);

    printf("Lendo TCP ..\n");
    fread(&struct_tcp, sizeof(tcp_hdr_t), 1, file);
    printf("    --> Porta de Origem: %d\n", ntohs(struct_tcp.sport));
    printf("    --> Porta de Destino: %d\n", ntohs(struct_tcp.dport));
    printf("    --> Tamanho do cabeçalho: %d bytes\n", (struct_tcp.hdr_len * 4));

    fseek(file, (struct_tcp.hdr_len * 4) - sizeof(tcp_hdr_t), SEEK_CUR);

    printf("Lendo Dados (HTTP) ..\n");
    int tam_dados = ntohs(struct_ip.tot_len) - (struct_ip.hdr_len*4) - (struct_tcp.hdr_len*4);
    printf("    --> Tamanho dos dados: %d bytes\n", tam_dados);
    printf("    --> Dados:\n");

    while(feof(file) == false) {
        char c = fgetc(file);
        printf("%c", c);
    }

    fclose(file);
}