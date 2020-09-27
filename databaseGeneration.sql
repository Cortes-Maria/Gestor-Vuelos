
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

create table TipoTarifa (
	pkTipoTarifa int primary key auto_increment not NULL,
    Tipo varchar(4),
    Precio decimal
);

create table Vuelo (
	pkVuelo int primary key not NULL,
    fkOrigen int,
    fkDestino int,
    fkAvion int,
    Salida datetime,
    Llegada datetime,
    fkTarifa int,
    TotalAsientos int,
    foreign key (fkOrigen) references Ciudad(pkCiudad),
    foreign key (fkDestino) references Ciudad(pkCiudad),
    foreign key (fkTarifa) references TipoTarifa(pkTipoTarifa)
);

create table Asiento (
	pkAsiento int primary key auto_increment not NULL,
    fkVuelo int,
    Tipo varchar(1),
    Fila int,
    Estado varchar(1),
    foreign key (fkVuelo) references Vuelo(pkVuelo)
);

create table Operario (
	pkOperario int primary key auto_increment not NULL,
    Usuario varchar(40),
    Contrasenna varchar(50)
);
