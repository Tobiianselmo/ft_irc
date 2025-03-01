# ft_irc

# Introduccion

Este documento describe el protocolo IRC, un sistema de chat basado en texto que opera en una red distribuida de servidores.

# Conceptos de IRC

- **Arquitectura**: Incluye servidores, clientes, servicios y operadores que mantienen la red y gestionan la comunicación.
- **Canales**: Espacios donde los usuarios se reúnen para conversar. Cada canal puede tener operadores que gestionan su funcionamiento.
- **Tipos de comunicación**: Interacciones uno a uno, uno a muchos (mensajes a canales) y comunicaciones de difusión general.

# Estructura del Protocolo

- **Formato de mensajes**: Describe la estructura básica de los mensajes en IRC, incluyendo etiquetas, origen, comandos y parámetros.
- **Respuestas numéricas**: Códigos numéricos que los servidores utilizan para comunicar diversos estados y respuestas a los clientes.
- **Expresiones comodín**: Uso de caracteres comodín para facilitar la coincidencia de patrones en nombres de canales y usuarios.

# Registro y Negociación de Capacidades

- **Registro de conexión**: Proceso mediante el cual un cliente se conecta y autentica en un servidor IRC.
- **Publicidad de características**: Métodos que los servidores emplean para informar a los clientes sobre las funcionalidades disponibles.
- **Negociación de capacidades**: Permite a clientes y servidores acordar el uso de características opcionales o extendidas.

# Mensajes del Cliente

- **Mensajes de conexión**: Comandos como `CAP`, `AUTHENTICATE`, `PASS`, `NICK`, `USER`, `PING`, `PONG`, `OPER`, `QUIT` y `ERROR`.
- **Operaciones de canal**: Comandos como `JOIN`, `PART`, `TOPIC`, `NAMES`, `LIST`, `INVITE` y `KICK`.
- **Consultas y comandos al servidor**: Comandos como `MOTD`, `VERSION`, `ADMIN`, `CONNECT`, `LUSERS`, `TIME`, `STATS`, `HELP`, `INFO` y `MODE`.
- **Envío de mensajes**: Uso de `PRIVMSG` y `NOTICE` para enviar mensajes privados o públicos.
- **Consultas basadas en usuarios**: Comandos como `WHO`, `WHOIS` y `WHOWAS`.
- **Mensajes de operador**: Comandos `KILL`, `REHASH`, `RESTART` y `SQUIT`.
- **Mensajes opcionales**: Incluyen `AWAY`, `LINKS`, `USERHOST` y `WALLOPS`.

# Tipos de Canales y Modos

- **Tipos de canales**: Canales regulares (`#`) y locales (`&`).
- **Modos de usuarios**: Configuraciones como modo invisible o de operador.
- **Modos de canal**: Configuraciones que determinan el comportamiento y restricciones dentro de un canal.
- **Prefijos de membresía**: Indican niveles de privilegio dentro de un canal.

# Respuestas Numéricas

Listado detallado de códigos numéricos utilizados por los servidores para comunicar estados, errores y respuestas a los clientes.

# Formato y Protocolos Adicionales

- **Formato de mensajes**: Uso de caracteres especiales para estilos como negrita, cursiva, subrayado y colores.
- **Protocolo Cliente a Cliente (CTCP)**: Extensión para enviar mensajes especiales.
- **Protocolo Directo de Cliente a Cliente (DCC)**: Transferencias directas de archivos y comunicaciones privadas sin pasar por el servidor.

# Operaciones de Canal

### JOIN

```markdown
Command: JOIN
Parameters: <channel>{,<channel>} [<key>{,<key>}]
Alt Params: 0
```

Permite unirse a canales específicos, creando el canal si no existe. Si es exitoso, el servidor envía mensajes de bienvenida y listas de usuarios activos.

Errores comunes:
- **ERR_NEEDMOREPARAMS (461)**: Faltan parámetros.
- **ERR_BANNEDFROMCHAN (474)**: Usuario baneado del canal.
- **ERR_INVITEONLYCHAN (473)**: Canal de solo invitación.
- **ERR_BADCHANNELKEY (475)**: Clave incorrecta.

### KICK

```markdown
Command: KICK
Parameters: <channel> <user> [<comment>}]
```

Expulsa a un usuario de un canal. Solo los operadores pueden ejecutar este comando.

Errores comunes:
- **ERR_USERNOTINCHANNEL (441)**: Usuario no está en el canal.
- **ERR_CHANOPRIVSNEEDED (482)**: Se requieren privilegios de operador.

### INVITE

```markdown
Command: INVITE
Parameters: <nickname> <channel>
```

Permite invitar a usuarios a un canal de acceso restringido.

Errores comunes:
- **ERR_NOSUCHNICK (401)**: Usuario no encontrado.
- **ERR_USERONCHANNEL (443)**: Usuario ya está en el canal.

### TOPIC

```markdown
Command: TOPIC
Parameters: <channel> [<topic>]
```

Muestra o cambia el tema de un canal. Requiere permisos de operador en algunos casos.

Errores comunes:
- **ERR_NOTONCHANNEL (442)**: Usuario no está en el canal.
- **ERR_CHANOPRIVSNEEDED (482)**: No tiene permisos para cambiar el tema.

# Consultas y Comandos del Servidor

### MODE

```markdown
Command: MODE
Parameters: <target> {[+|-]<mode>} [<mode arguments>]
```

Permite establecer o consultar modos de usuario y canal.

Ejemplos:
- `+i`: Modo invisible.
- `+s`: Recibir notificaciones del servidor.
- `+o`: Otorgar privilegios de operador.

Errores comunes:
- **ERR_NOSUCHNICK (401)**: Usuario no encontrado.
- **ERR_NOSUCHCHANNEL (403)**: Canal no encontrado.

---

