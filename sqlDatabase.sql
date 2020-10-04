create database gestorVuelos;
use gestorVuelos;

create table Cliente (
	pkCliente int primary key not NULL,
    Nombre varchar(40),
    Apellido1 varchar(40),
    Apellido2 varchar(40),
    Sexo varchar(1),
    FechaNacimiento datetime,
    Edad varchar(1),
    CHECK ((Sexo='F' or Sexo='M') AND (Edad='A' or Edad='I'))
);

create table CuentaCliente (
	fkCliente int not NULL,
    Usuario varchar(40),
    Contrasenna varchar(50),
    FOREIGN KEY (fkCliente) REFERENCES Cliente(pkCliente)
);

create table Marca (
	pkMarca int primary key auto_increment not NULL,
    Descripcion varchar(50)
);

create table Modelo (
	pkModelo int primary key auto_increment not NULL,
    descripcion varchar(50),
    fkMarca int,
    foreign key (fkMarca) references Marca(pkMarca)
);

create table Avion (
	pkAvion int primary key not NULL,
    fkModelo int,
    Anno int,
    foreign key (fkModelo) references Modelo(pkModelo)
);

create table Ciudad (
	pkCiudad int primary key auto_increment not NULL,
    Codigo varchar(40)
);

create table Vuelo (
	pkVuelo int primary key not NULL,
    fkOrigen int,
    fkDestino int,
    fkAvion int,
    Salida datetime,
    Llegada datetime,
    TotalAsientos int,
    foreign key (fkOrigen) references Ciudad(pkCiudad),
    foreign key (fkDestino) references Ciudad(pkCiudad),
    foreign key (fkAvion) references Avion(pkAvion)
);

create table Tarifa (
	pkTarifa int primary key auto_increment not NULL,
    fkVuelo int,
    Tipo varchar(4),
    Precio decimal,
    foreign key (fkVuelo) references Vuelo (pkVuelo)
);

create table Asiento (
	pkAsiento int primary key auto_increment not NULL,
    fkVuelo int,
    Tipo varchar(2),
    Fila int,
    Columna int,
    Estado varchar(1),
    foreign key (fkVuelo) references Vuelo(pkVuelo)
);


create table Operario (
	pkOperario int primary key auto_increment not NULL,
    Usuario varchar(40),
    Contrasenna varchar(50)
);

create table Aerolinea (
	nombre varchar(80),
    NumHub int
);

create table Reserva (
	pkReserva int primary key auto_increment,
    fkCliente int,
    fkVuelo int,
    fecha datetime,
    foreign key (fkCliente) references Cliente (pkCliente),
    foreign key (fkVuelo) references Vuelo (pkVuelo)
);

create table ClienteXReserva (
	fkReserva int,
    fkCliente int,
    fkAsiento int,
    primary key (fkReserva, fkCliente, fkAsiento),
    foreign key (fkReserva) references Reserva (pkReserva),
    foreign key (fkCliente) references Cliente (pkCliente),
    foreign key (fkAsiento) references Asiento (pkAsiento)
);

-- DROPS

DROP TABLE ClienteXReserva;
DROP TABLE Reserva;
DROP TABLE Asiento;
DROP TABLE Tarifa;
DROP TABLE Vuelo;
DROP TABLE Ciudad;
DROP TABLE Avion;
DROP TABLE Modelo;
DROP TABLE Marca;
DROP TABLE CuentaCliente;
DROP TABLE Cliente;
DROP TABLE Operario;
DROP TABLE Aerolinea;

select * from Avion;