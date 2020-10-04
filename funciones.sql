use gestorVuelos;
select * from Avion

DELIMITER //
CREATE FUNCTION InsertarAvion(modelo int, matricula int, anno int)
RETURNS int
BEGIN	
    insert into Avion (pkAvion, fkModelo, Anno)
    values (matricula, modelo, anno);
	RETURN 1;
END
//


DELIMITER //
CREATE FUNCTION existeAvion(matricula int)
RETURNS int
BEGIN
	DECLARE cantAvion int DEFAULT 0;
	SET cantAvion = (SELECT COUNT(Avion.pkAvion)
	FROM Avion WHERE Avion.pkAvion = matricula);
	IF cantAvion > 0
	THEN RETURN 1;
    ELSE RETURN 0;
    END IF;
END
//



DELIMITER //
create procedure verAvion(matricula int)
BEGIN
	SELECT Avion.pkAvion, Modelo.descripcion, Marca.Descripcion
    FROM Avion inner join Modelo on Avion.fkModelo = Modelo.pkModelo inner join Marca on Modelo.fkMarca = Marca.pkMarca
    WHERE Avion.pkAvion = matricula;
END
//

DELIMITER //
CREATE FUNCTION esAdulto(pkCliente int)
RETURNS int
BEGIN
	DECLARE edad int DEFAULT 0;
    SET edad =
    (SELECT YEAR(CURDATE())-YEAR(Cliente.FechaNacimiento) + IF(DATE_FORMAT(CURDATE(),'%m-%d') > DATE_FORMAT(Cliente.FechaNacimiento,'%m-%d'), 0 , -1 ) 
    FROM Cliente 
    WHERE Cliente.pkCliente = pkCliente);

	IF edad > 12
	THEN RETURN 1;
    ELSE RETURN 0;
    END IF;
END
//

DELIMITER //
CREATE FUNCTION AvionVuelo(matricula int)
RETURNS int
BEGIN
	DECLARE cantAvion int DEFAULT 0;
	SET cantAvion = (SELECT COUNT(Avion.pkAvion)
	FROM Avion inner join Vuelo on Avion.pkAvion = Vuelo.fkAvion WHERE Avion.pkAvion = matricula);
	IF cantAvion > 0
	THEN RETURN 1;
    ELSE RETURN 0;
    END IF;
END
//

DELIMITER //
create procedure TOP3_MayorVenta()
BEGIN
	SELECT obtener_monto(Reserva.pkReserva) as Monto, Vuelo.pkVuelo
    FROM Reserva inner join Vuelo on Reserva.fkVuelo = Vuelo.pkVuelo
    ORDER BY Monto ASC
    LIMIT 3;
END
//

