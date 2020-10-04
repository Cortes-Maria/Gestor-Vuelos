-- Marcas
insert into Marca(Descripcion)
values ('BOEING');

insert into Marca(Descripcion)
values ('AIRBUS');

insert into Marca(Descripcion)
values ('EMBRAER');

--   BOEING modelos
insert into Modelo(descripcion, fkMarca)
values ('6D', 1);

insert into Modelo(descripcion, fkMarca)
values ('7J7', 1);

insert into Modelo(descripcion, fkMarca)
values ('GA-1', 1);

insert into Modelo(descripcion, fkMarca)
values ('737 Classic', 1);

insert into Modelo(descripcion, fkMarca)
values ('NC-135', 1);


-- AIRBUS modelos
insert into Modelo(descripcion, fkMarca)
values ('A300', 2);

insert into Modelo(descripcion, fkMarca)
values ('Beluga XL', 2);

insert into Modelo(descripcion, fkMarca)
values ('A340', 2);

insert into Modelo(descripcion, fkMarca)
values ('A330neo', 2);

insert into Modelo(descripcion, fkMarca)
values ('NSR', 2);

-- EMBRAER modelos
insert into Modelo(descripcion, fkMarca)
values ('EMB 110 Bandeirante', 3);

insert into Modelo(descripcion, fkMarca)
values ('EMB 120 Brasilia', 3);

insert into Modelo(descripcion, fkMarca)
values ('EMB 121 Xingú', 3);

insert into Modelo(descripcion, fkMarca)
values ('FMA CBA 123 Vector', 3);

insert into Modelo(descripcion, fkMarca)
values ('ERJ 145', 3);



-- INSERTS PARA INICIAR SESIÓN

-- CLIENTE y CuentaCliente
insert into Cliente(pkCliente, Nombre, Apellido1, Apellido2, Sexo, FechaNacimiento, Edad)
values (274628, 'Ignacio', 'Carrera', 'Rodriguez', 'M', '1980-05-22', 'A');
insert into CuentaCliente(fkCliente, Usuario, Contrasenna)
values (274628, 'Igcaro', 1234);

insert into Cliente(pkCliente, Nombre, Apellido1, Apellido2, Sexo, FechaNacimiento, Edad)
values (182959, 'Daniela', 'Montenegro', 'Sanabria', 'F', '1977-02-10', 'A');
insert into CuentaCliente(fkCliente, Usuario, Contrasenna)
values (182959, 'DaniM', 12345);

insert into Cliente(pkCliente, Nombre, Apellido1, Apellido2, Sexo, FechaNacimiento, Edad)
values (315391, 'María', 'Solorzano', 'Fernandez', 'M', '2018-12-11', 'I');
-- Operario
insert into Operario (pkOperario, Usuario, Contrasenna)
values (312, 'mainAdmin', 4321);



-- INSERTS PARA CREAR RESERVACIONES Y UN VUELO

-- CIUDAD
insert into Ciudad (Codigo) values ('SJO');
insert into Ciudad (Codigo) values ('LIM'); 

-- AVION
insert into Avion values (1, 1, 2010);

-- VUELO
insert into Vuelo values (1, 1, 2, 1, '2020-10-01 22:00', '2020-10-02 08:00', 30);

-- TARIFAS
insert into Tarifa (fkVuelo, Tipo, Precio) values (1, 'BA', 500);
insert into Tarifa (fkVuelo, Tipo, Precio) values (1, 'BI', 200);
insert into Tarifa (fkVuelo, Tipo, Precio) values (1, 'SA', 300);
insert into Tarifa (fkVuelo, Tipo, Precio) values (1, 'SI', 100);
insert into Tarifa (fkVuelo, Tipo, Precio) values (1, 'EA', 200);
insert into Tarifa (fkVuelo, Tipo, Precio) values (1, 'EI', 50);

-- PARA CREAR LOS ASIENTOS DEL VUELO, COMPILAR EL PROCEDIMIENTO INSERTA_ASIENTOS Y EJECUTAR LO SIGUIENTE
CALL inserta_asientos(1, 'E', 2, 5, 1);
CALL inserta_asientos(1, 'S', 2, 5, 3);
CALL inserta_asientos(1, 'B', 2, 5, 5);
insert into Asiento (fkVuelo, Tipo, Fila, Columna, Estado)
	Values (1, 'BI', 0, 0, 'I'); -- Este asiento será 'ocupado' por todos los infantes
insert into Asiento (fkVuelo, Tipo, Fila, Columna, Estado)
	Values (1, 'SI', 0, 0, 'I'); -- Este asiento será 'ocupado' por todos los infantes
insert into Asiento (fkVuelo, Tipo, Fila, Columna, Estado)
	Values (1, 'EI', 0, 0, 'I'); -- Este asiento será 'ocupado' por todos los infantes
select * from Asiento;

-- Reserva ID: 1
select * from Reserva;
INSERT INTO Reserva (fkCliente, fkVuelo, fecha) VALUES (182959, 1, '2020-09-02 21:44:37');
INSERT INTO ClienteXReserva VALUES (1, 182959, 26);
INSERT INTO ClienteXReserva VALUES (1, 205620, 27);
INSERT INTO ClienteXReserva VALUES (1, 233414, 28);
INSERT INTO ClienteXReserva VALUES (1, 999923, 15);
update Asiento SET Estado = 'O' where pkAsiento = 26;
update Asiento SET Estado = 'O' where pkAsiento = 27;
update Asiento SET Estado = 'O' where pkAsiento = 28;
update Asiento SET Estado = 'O' where pkAsiento = 15;

select * from Asiento;
-- Reserva ID: 2
INSERT INTO Reserva (fkCliente, fkVuelo, fecha) VALUES (274628, 1, '2020-09-03 16:23:37');
INSERT INTO ClienteXReserva VALUES (2, 274628, 6);
INSERT INTO ClienteXReserva VALUES (2, 9233715, 7);
INSERT INTO ClienteXReserva VALUES (2, 315391, 33);
select * from ClienteXReserva;
update Asiento SET Estado = 'O' where pkAsiento = 6;
update Asiento SET Estado = 'O' where pkAsiento = 7;
