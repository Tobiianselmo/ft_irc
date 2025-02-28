# ft_irc

Parsear los comandos:

la mayoria de los comandos necesitan que el usuario este autenticado.
Hay tres estados, No-autenticado, Autenticado y registrado.

## Comandos:
## Mensajes del cliente

Los mensajes son de cliente a servidor a menos que sea especificamente dicho. Si un mensaje es enviado desde el servidor al cliente conectado, tiene que notarse en la descripcion del mensaje. 

En la descripcion de los mensajes pueden estar:
         - "command" se refiere al comportamiento cuando se envia desde un cliente al servidor. 
         - "Ejemplos de comandos" representa mensajes de ejemplo enviados *al servidor por un cliente*
         - "Ejemplos de mensajes" representa ejemplos de mensajes enviados *al cliente por el servidor*. 
        Si un comando es enviado por un cliente a un servidor con menos parametros de los que un comando necesia para ser procesado, el servidor para contestar con un error numero y fallará.

### CHANEL OPERATIONS:
  Este tipo de mesajes se refiere a todo lo que tenga que ver con manipular canales, sus propiedades y contenidos.
  *Son pedidos al servidor, quien tiene que aprobar o desaprovar la peticion*, si es aprobada se envia un mensaje conteniendo la misma informacion al cliente.

  NÚMEROS DE ERROR o que solo sirven de cosas :)
  RPL_INVITING = 341
  RPL_NOTOPIC = 331
  RPL_TOPIC = 332
  RPL_TOPICWHOTIME = 333
  RPL_NAMREPLY = 353
  RPL_ENDOFNAME = 366
  ERR_NEEDMOREPARAMS = 461
  ERR_NOSUCHCHANNEL = 403
  ERR_TOOMANYCHANNELS = 405
  ERR_BADCHANNELKEY = 475
  ERR_CHANNELISFULL = 471
  ERR_INVITEONLYCHAN = 473
  ERR_NOTONCHANNEL = 442
  ERR_CHANNOPRIVSNEEDED =  482
  ERR_USERONCHANNEL = 443
  
  ### KICK message:
  Command: KICK
  Parameters: <channel> <user> *( ";" <user> ) [<comment>]
  *Cuando dice user se refiere al nickname en nuestro código*
  El comando KICK es utilizado para pedir que se remueva un usuario del canal. Obliga al <user> a ser echado a la fuerza.
  Puede ser enviad desde el servidor al canal para notificar que alguien fue removido del canal. Aqui el mensaje <source> va a ser el cliente quien envio el comando kick y <channel> va a ser el canal del que el target fue echado.
  Puede agregarse un mensaje de "explicación" sino se debe enviar un mensaje default.
  El servidor no debe enviar a multiples usuarios en este mensaje a los clientes y DEBE distribuir este mensaje kick de multiples usuarios como una serie de mensajes con un unico nombre en cada uno. 
  Un servidor puede limitar el numero de usuarios targer por cada comando kick, y de manera *silenciosa* dejar targets si el numero de targets es más grande que el limite.
  Ejemplos:
     - KICK #Finish Matthew
     
  ### INVITE message
  Command: INVITE
  Parameters: <nickname> <channel>
  Es usado para invitar un usuario al canal.
  El canal DEBE existir y que por lo menos tenga un usuario. De otra manera el servidor debe rechazar el comando con el error numerico de ERR_NOSUCHCHANNEL.
  Solo los miembros del canal pueden invitar a otros usuarios. De otra manera debe ser rechazado el comando y enviar el error numerico de ERR_NOTONCHANNEL.
  El servidor puede rechazarla cuando el channel sea _invite-only_ mode y el usuario no sea el administrador del canal con el numero ERR_CHANOPRIVSNEEDED.
  Si el usuario ya está en el canal el servidor debe ser rechazar la operacion con el error numerico ERR_USERONCHANNEL.
  Cuando sale *todo bien* el servidor debe enviar un númeor RPL_INVITING a quien envio la solicitud y un mensaje INVITE con el nombre del usuario que los invitó como <source>. *Los otros miembros del canal no deben ser notificados* (lo cual está muy feo)

  Ejemplo:
  INVITE Wiz #river

  Invite list:
  El servidor tal vez permita la invitacion con ningun parametro y que conteste con una lista de canales al que el usuario está invitado/incluido.
  (Servers MAY allow the INVITE with no parameter, and reply with a list of channels the sender is invited to as RPL_INVITELIST (336) numerics, ending with a RPL_ENDOFINVITELIST (337) numeric. *esto es de donde lo saqué pero no estoy segura de la traducción asi que consultenlo*)

  ### TOPIC Message
  Command: TOPIC
  Parameters: <channel> [<topic>]
  El comando topic es usafo para cambiar o mirar el _tema_ del canal recibido por parametros. Si no se da un <topic>, puede retornar RPL_TOPIC o RPL_NOTOPIC especificando el tema del canal actual o si no hay uno. Si se da un <topic> vacio se va a eliminar el tema del canal.
  Si el usuario no es parte del canal del cual quiere ver el topic el servidor debe retornar el error numerico ERR_NOTONCHANNEL y que falle le comando
  Si se tiene el mode _protected topic_ está en el canal, entonces los clientes tienen que tener los permisos apropiados del canal para poder modificarlo. Si el cliente no los tiene, debe salir el error ERR_CHANOPRIVSNEEDED y fallar el comando.
  Si el tema es cambiado o eliminado, todos los clientes (incluido quien llamó al comando) del canal van a recibir un comando TOPIC con el nuevo tema como arguemento (o un argumento vacio si ese es el caso).
  Si el argumento <topic> viene igual al tema ya declarado, el servidor puede enviar un mensaje igualmente al autor y/o a los otros usuarios.
  *Cuando los clientes se unan al canal van a recibir un número RPL_TOPIC (o si no lo hay) de manera apropiada.

  ### JOIN message
  Command: JOIN
  Parameters: <channel>{,<channel>} [<key>{,<key>}]
  Alt Params: 0
  El comando JOIN indica que el cliente quiere unirse a los canales enviados, a cada canal debe ser usando la key o keys correspodientes. El servidor que reciba el comando debe revisar que el cliente pueda unirse al canal y procesar el pedido. El servidor DEBE procesar los parametros del comando como una lista de comandos enviado por los clientes. 
  <key Nº 1> => <channel Nº 1>
  <key Nº 2> => <channel Nº 2>, etc.
  Mientras que el cliente este dentro del canal, debe recibir toda la informacion correspondiente sobre el canal, incluyendo los JOIN, PART, KICK y MODE comandos afectando al canal. Reciben los mensajes PRIVMSG y NOTICE enviados al canal y los mensajes QUIT de parte de otros clientes que se unieron al mismo canal (para hacerle saber esos usuarios dejaron e canal y la network. Para así saber sobre los miembros del canal y sus modos.
  Cuando el comando funciona el servidor DEBE enviar *en este orden*
  1. Un mensaje JOIN con el cliente como <source> y el canal al que se han unido como primer parametro del mensaje.
  2. El tema del canal (con RPL_TOPIC y opcionalmente RPL_TOPICWHOTIME) y ningun mensaje si el canal no tiene tema.
  3. Una lista con todos los miembros que están en el canal (con uno o más RPL_NAMEREPLY seguidos por un solo RPL_ENDOFNAMES). Estos RPL_NAMEREPLY enviados por el servidor deben incluir al cliente que se acaba de unir al canal.

  Los modos de los canales _key, client limit, ban - exception, invite-only -exception_ van a afecta a si el cliente puede unirse al canal.
  Los servidores pueden llegar a restringir el numero de canales al que un cliente puede unirsea al mismo tiempo. (revisar info)
  Tambien hay que tener en cuenta que el comando acepta argumentos especiales (0) en vez de los usuales, lo que significa que el cliente que lo pidio quiere salir de todos los canales en el que está. El servidor va a procesar este comando como que el cliente envio un PART command a todos los canales de los cuales es miembro.

  Este mensaje puede ser enviado por el servidor a un cliente para notificarle que alguien más se a unido al canal. En este caso <source> va a ser el del cliente quien se está uniendo y <channel> va a ser el canal al que se une. *El servidor no tendria que enviar multiples canales en este mensaje, tendría que distribuir ese mensaje a cada canal con su propio nombre unicamente.*
  
  Ejemplos:
  JOIN #foobar ; se unio al canal
  JOIN #foo fubar ; se unio al canal usando la key fubar
  JOIN #foo,&bar fubar,foobar ; entienden la situacion :)

Canales:

Cada canal tiene que ser identificado con un nombre con pre-fijo # (#general) y puede tener muchos atributos:
      - tema
      - lista de operadores/administradores
      - limite de usuarios
      - contraseña
      - acceso registrado.
El canal tine que tener una lista de miembros para saber qué usuarios estan o no en el canal para asegurarse que las reglas del grupo se cumplan.

Los canales tambien manejan el flujo de mensajes. Cuando un mensaje se envia el canal debe asegurarse que todos los miembros del canal lo reciban.

Los canales pueden ser:
       - Abierto para todos.
       - Unicamente con invitación.
       - Con contraseña.
(se aplican de manera distinta, de iniciación o con el MODE comando)

Clientes:

En este contexto los clientes son una representación de la coneccion con el servidor, abstraida a una clase que maneja el estado de cada usuario interactuando con la network de irc.

El cliente es más como algo que guarda la informacion, asegurandose que las comunicaciones sucedan como se espera.

La clase cliente maneja toda la información crucial con el usuario:
       - nickname:
       No pueden contener:
           - ' ' (espacios)
           - , (comas)
           - ¿? (signos de pregunta)
           - ! (signos de exclamación)
           - @ (arroba)
           - .
          *Se debe evitar el uso de nicknames ambiguos con los comandos            o un nick que sea igual a un comando*
      No deben comenzar con:
           - $
           - ;
      No deben comenzar con ninguno de los caracteres que comienzan con         cosas referidas a los canales:
           Tipos de canales: # (son para canales regulares), & (canales              locales).
           Prefijos de miembros de canales: *~/+q* (Prefijo de fundador),            *&/+a* (Prefijo de protección), *@/+o* (Prefijo de operador),             *%/+h* (halfop prefix, no sé traducirlo jajajaj), *+/+v*                  (prefijo de voz).
       - username
       - hostname
       - real name
       - FD
       - Estado de autenticacion.

A medida que los usuarios se manejan por el servidor la clase Cliente se asegura de registrar y reinforzar los estados para que todas las operaciones funcionen.

Esta clase tambien se encarga de manejar en que canales el usuario esta en el momento, manejando el recibir y enviar mensajes, manteniendo un correcto estado de las actividades del usuario en el servidor. Cuando un usuario te envia un mensaje, el cliente debe asegurarse que el mensaje este bien formateado y enviado a los receptores, ya sean usuarios o canales.
