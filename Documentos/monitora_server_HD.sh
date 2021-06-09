#!/bin/bash
##########################################
#       by Alexandre Bargiela            #
#    Testado no debian 4.0 Stable        #
# Necessita o pacote sendEmail instalado #
#    Para um funcionamento correto       #
########################################## 
ESPACO=`df -h | awk '{print $5}' | grep -v Use | sort -nr | awk -F % '{print $1}' | head -n1`
ESPACO_INODE=`df -hi | awk '{print $5}'  | grep -v IUse | sort -nr | awk -F % '{print $1}' | head -n1`
ALERTA_RISCO="O disco esta prestes ficar sem espaco por favor verificar \n\n `df -h ` \n\n `ifconfig` \n\n `uname -a`"
ALERTA_CHEIO="O disco encheu verificar com urgencia \n\n `df -h ` \n\n `ifconfig`\n\n  `uname -a`"
ALERTA_RISCO_INODE="O disco esta prestes ficar sem espaco por favor verificar \n\n `df -hi ` \n\n `ifconfig` \n\n `uname -a`"
ALERTA_CHEIO_INODE="O disco encheu verificar com urgencia \n\n `df -hi ` \n\n `ifconfig`\n\n  `uname -a`"
SUBJECT_1="ESPACO EM DISCO PREOCUPANTE"
SUBJECT_2="ESPACO EM DISCO ACABOU"
SUBJECT_1_I="ESPACO EM DISCO PREOCUPANTE_INODE"
SUBJECT_2_I="ESPACO EM DISCO ACABOU_INODE"
EMAIL_DST="voce@xx.com.br"# ALTERAR PARA SEU EMAIL
EMAIL_ORI="sistema@xx.com.br" #ALTERAR PARA O USUARIO DO SISTEMA QUE ENVIARA
SERVER="server.com.br" #ALTERAR PARA O SEU SERVER

# Verifica espaco em disco
case $ESPACO in
        9[1-9])# Caso o disco esteja entre 91 a 99 ele manda um email avisando
        sendEmail -f $EMAIL_ORI -t $EMAIL_DST -u $SUBJECT_1 -m "$ALERTA_RISCO" -s $SERVER:25;;
        100)# Caso encha tb
        sendEmail -f $EMAIL_ORI -t $EMAIL_DST -u $SUBJECT_2 -m "$ALERTA_CHEIO"  -s $SERVER:25;;
esac

case $ESPACO_INODE in
        9[1-9])# Caso o disco esteja entre 91 a 99 ele manda um email avisando
        sendEmail -f $EMAIL_ORI -t $EMAIL_DST -u $SUBJECT_1_I -m "$ALERTA_RISCO_INODE" -s $SERVER:25;;
        100)# Caso encha tb
        sendEmail -f $EMAIL_ORI -t $EMAIL_DST -u $SUBJECT_2_I  -m "$ALERTA_CHEIO_INODE"  -s $SERVER:25;;
esac
