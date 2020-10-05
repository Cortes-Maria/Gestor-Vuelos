use gestorVuelos;
-- FUNCION PARA INICIAR SESION
DELIMITER //
CREATE FUNCTION verificar_usuario(v_usuario varchar(20), v_contrasenna varchar(150))
RETURNS INT
BEGIN
	DECLARE v_username varchar(20);

	IF (SELECT SUBSTRING(v_usuario, 1, 6)) = 'admin/' THEN
		select Usuario into v_username
			from Operario
			where Usuario = (SELECT SUBSTRING(v_usuario, 7)) and Contrasenna = v_contrasenna;
		IF v_username IS NULL THEN
			RETURN 1;
		ELSE
			RETURN 2;
		END IF;
	ELSE
		select Usuario into v_username
			from CuentaCliente
			where Usuario = v_usuario and Contrasenna = v_contrasenna;
		IF v_username IS NULL THEN
			RETURN 1;
		ELSE
			RETURN 3;
		END IF;
	END IF;
END
//
-- PROCEDIMIENTO QUE INSERTA ASIENTOS 
DELIMITER // 
CREATE PROCEDURE inserta_asientos(v_idVuelo int, v_tipo_asiento varchar(3), v_filas int, v_cantidad int, v_inicio_fila int)
BEGIN -- v_filas: cantidad de filas, v_cantidad: asientos por fila, v_inicio_fila: fila por donde empezar
	DECLARE asientos int;-- Total de asientos a insertar
    DECLARE contador_columna int;-- Cuenta las columnas
    DECLARE contador_total int;-- Cuentas hasta llegar al total de asientos
    SET contador_columna = 1;
    SET contador_total = 1;
    SET asientos = v_filas*v_cantidad;
	bucle: LOOP
        IF contador_total > asientos THEN
			LEAVE bucle;
		END IF;
        INSERT INTO Asiento (fkVuelo, Tipo, Fila, Columna, Estado) values (v_idVuelo, v_tipo_asiento, v_inicio_fila, contador_columna, 'L');
        IF contador_columna = v_cantidad THEN
			SET contador_columna = 0;
            SET v_inicio_fila = v_inicio_fila + 1;
		END IF;
        SET contador_total = contador_total + 1;
        SET contador_columna = contador_columna + 1;
	END LOOP;
END
//

-- FUNCION QUE RETORNA EL MONTO TOTAL DADA UNA RESERVACION
DELIMITER //
CREATE FUNCTION obtener_monto(v_idReservacion int)
RETURNS INT
BEGIN
	DECLARE monto int;
    
    select sum(t.precio) into monto
	from Cliente c
		inner join ClienteXReserva cXr on c.pkCliente = cXr.fkCliente
        inner join Asiento a on cXr.fkAsiento = a.pkAsiento
        inner join Vuelo v on a.fkVuelo = v.pkVuelo
        inner join Tarifa t on v.pkVuelo = t.fkVuelo
	where SUBSTRING(t.Tipo,1,1) = SUBSTRING(a.Tipo, 1, 1)
		and SUBSTRING(t.Tipo, 2) = c.Edad
        and cXr.fkReserva = v_idReservacion;
	IF monto IS NULL THEN
		RETURN 0;
	END IF;
    
	RETURN monto;
END
//

select obtener_monto(45);
DELIMITER //
CREATE PROCEDURE obtener_query_r(idReserva int)
BEGIN
    SELECT a.fkVuelo, c.Nombre, c.Apellido1, c.Apellido2, c.pkCliente, a.Tipo, a.Fila, a.Columna, t.Precio, c.Edad
				FROM Cliente c
					INNER JOIN ClienteXReserva cXr ON c.pkCliente = cXr.fkCliente
					INNER JOIN Asiento a ON cXr.fkAsiento = a.pkAsiento
					INNER JOIN Vuelo v ON a.fkVuelo = v.pkVuelo
                    INNER JOIN Tarifa t ON v.pkVuelo = t.fkVuelo
				where SUBSTRING(t.Tipo,1,1) = SUBSTRING(a.Tipo, 1, 1)
				and SUBSTRING(t.Tipo, 2) = c.Edad
                and cXr.fkReserva = 2;idReserva;
END
//
    
DELIMITER //
CREATE PROCEDURE obtener_query_v(idQuery int, idVuelo int)
BEGIN

	IF idQuery = 0 THEN -- Información general del vuelo
		SELECT c1.Codigo Ciudad_salida, v.Salida, c2.Codigo Ciudad_destino, v.LLegada, ma.Descripcion Marca, mo.descripcion Modelo
		from Vuelo v
		inner join Ciudad c1 on v.fkOrigen = c1.pkCiudad
		inner join Ciudad c2 on v.fkDestino = c2.pkCiudad
		inner join Avion a on v.fkAvion = a.pkAvion
		inner join Modelo mo on a.fkModelo = mo.pkModelo
		inner join Marca ma on mo.fkMarca = ma.pkMarca
		where v.pkVuelo = idVuelo;
        
    ELSEIF idQuery = 1 THEN -- Asientos por tipo de vuelo
		select concat(Tipo, Estado) Tipo, count(pkAsiento) Cantidad
		from Asiento
		inner join Vuelo on fkVuelo = pkVuelo
		where Tipo <> 'BI' and Tipo <> 'SI' and Tipo <> 'EI'and pkVuelo = idVuelo 
        group by concat(Tipo, Estado) order by Tipo;
	ELSEIF idQuery = 2 THEN
		select concat(Tipo, Estado) Asiento, fila, columna
		from Asiento
		where Tipo <> 'BI' and Tipo <> 'SI' and Tipo <> 'EI' and fkVuelo = idVuelo
		order by fila, columna;
	ELSEIF idQuery = 3 THEN -- Indica cantidad de adultos e infantes por vuelo
		select c.Edad, count(v.pkVuelo) Total
		from Asiento a
		inner join ClienteXReserva cXr on cXr.fkAsiento = a.pkAsiento
		inner join Vuelo v on v.pkVuelo = a.fkVuelo
		inner join Cliente c on cXr.fkCliente = c.pkCliente
        where v.pkVuelo = idVuelo
        group by c.Edad order by c.Edad;
        
	END IF;
END
//

DELIMITER //
CREATE PROCEDURE obtener_asientos(idVuelo int)
BEGIN
	select concat(Tipo, Estado) Asiento, fila, columna
		from Asiento
		where Tipo <> 'BI' and Tipo <> 'SI' and Tipo <> 'EI' and fkVuelo = idVuelo
		order by fila, columna;
END
//

DELIMITER //
CREATE PROCEDURE info_general(idVuelo int)
BEGIN
	SELECT c1.Codigo Ciudad_salida, v.Salida, c2.Codigo Ciudad_destino, v.LLegada, ma.Descripcion Marca, mo.descripcion Modelo
		from Vuelo v
		inner join Ciudad c1 on v.fkOrigen = c1.pkCiudad
		inner join Ciudad c2 on v.fkDestino = c2.pkCiudad
		inner join Avion a on v.fkAvion = a.pkAvion
		inner join Modelo mo on a.fkModelo = mo.pkModelo
		inner join Marca ma on mo.fkMarca = ma.pkMarca
		where v.pkVuelo = idVuelo;
END
//

DELIMITER //
CREATE PROCEDURE tipo_asientos(idVuelo int)
BEGIN
	select concat(Tipo, Estado) Tipo, count(pkAsiento) Cantidad
		from Asiento
		inner join Vuelo on fkVuelo = pkVuelo
		where Tipo <> 'BI' and Tipo <> 'SI' and Tipo <> 'EI'and pkVuelo = idVuelo 
        group by concat(Tipo, Estado) order by Tipo;
END
//

DELIMITER //
CREATE PROCEDURE asiento_edades(idVuelo int)
BEGIN
	select c.Edad, count(v.pkVuelo) Total
		from Asiento a
		inner join ClienteXReserva cXr on cXr.fkAsiento = a.pkAsiento
		inner join Vuelo v on v.pkVuelo = a.fkVuelo
		inner join Cliente c on cXr.fkCliente = c.pkCliente
        where v.pkVuelo = idVuelo
        group by c.Edad order by c.Edad;
END
//

DELIMITER //
CREATE FUNCTION existeReserva(idReserva int)
RETURNS int
BEGIN
	DECLARE cantReserva int DEFAULT 0;
	SET cantReserva = (SELECT COUNT(r.pkReserva)
	FROM Reserva r WHERE r.pkReserva = idReserva);
	IF cantReserva > 0
	THEN RETURN 1;
    ELSE RETURN 0;
    END IF;
END
//

DELIMITER //
CREATE FUNCTION existeCliente(idCliente int)
RETURNS int
BEGIN
	DECLARE cantCliente int DEFAULT 0;
    DECLARE v_edad varchar(2);
	
    SET cantCliente = (SELECT COUNT(c.pkCliente)
					FROM Cliente c WHERE c.pkCliente= idCliente);
	IF cantCliente > 0 THEN
		IF (SELECT Edad FROM Cliente
				WHERE pkCliente = idCliente) = 'A' THEN RETURN 1;
        ELSE RETURN 2;
		END IF;
    ELSE RETURN 0;
    END IF;
END
//

CREATE FUNCTION existeVuelo(idVuelo int)
RETURNS int
BEGIN
	DECLARE cantVuelo int DEFAULT 0;
	SET cantVuelo = (SELECT COUNT(v.pkVuelo)
	FROM Vuelo v WHERE v.pkVuelo= idVuelo);
	IF cantVuelo > 0
	THEN RETURN 1;
    ELSE RETURN 0;
    END IF;
END
//

DELIMITER //
CREATE FUNCTION asiento_disponible(idVuelo int, v_fila int, v_columna int)
RETURNS INT
BEGIN
	DECLARE dispAs int DEFAULT 0;
    SET dispAs = (SELECT COUNT(a.pkAsiento)
					FROM Asiento a WHERE a.fkVuelo = idVuelo
					AND a.Fila = v_fila AND a.Columna = v_columna
                    AND Estado = 'L');
	IF dispAs > 0 THEN -- retorna el id del asiento
		RETURN (SELECT a.pkAsiento FROM Asiento a WHERE a.fkVuelo = idVuelo
					AND a.Fila = v_fila AND a.Columna = v_columna);
	ELSE
		RETURN 0;
	END IF;
END
//

DELIMITER //
CREATE FUNCTION retornaIDAsiento(idVuelo int, tipoAsiento varchar(3))
RETURNS int
BEGIN 
	DECLARE idAsiento int;
    SET idAsiento = (SELECT pkAsiento from Asiento where fkVuelo = idVuelo and Tipo = tipoAsiento);
	
    RETURN idAsiento;
END
//

DELIMITER //
CREATE FUNCTION crearReservacion(idCliente INT, idVuelo INT)
RETURNS INT
BEGIN
	DECLARE idReserva INT;
	INSERT INTO Reserva (fkCliente, fkVuelo, Fecha) VALUES (idCliente, idVuelo, (SELECT NOW()));
    SET idReserva = (SELECT max(pkReserva) FROM Reserva WHERE fkCliente = idCliente and fkVuelo = idVuelo);
    RETURN idReserva;
END
//

DELIMITER //
CREATE PROCEDURE insertaClientesReserva(idReserva INT, idCliente INT, idAsiento INT)
BEGIN
	INSERT INTO ClienteXReserva VALUES (idReserva, idCliente, idAsiento);
END
//

SELECT pkReserva FROM(
	SELECT max(pkReserva) FROM Reserva WHERE fkCliente = 182959 and fkVuelo = 2;
	ORDER BY pkReserva DESC;
) WHERE ROWNUM = 1;


DELIMITER //
CREATE TRIGGER asientosOcupados
AFTER INSERT ON ClienteXReserva
FOR EACH ROW
BEGIN
	update Asiento 
    SET Estado = 'O' where pkAsiento = new.fkAsiento;
END
//

DELIMITER //
CREATE FUNCTION personasXvuelo(idVuelo INT)
RETURNS INT
BEGIN
		DECLARE personasVuelo INT;
		SET personasVuelo = (select count(pkVuelo) from Vuelo v inner join 
							Asiento a on a.fkVuelo = v.pkVuelo inner join 
							ClienteXReserva cXr on cXr.fkAsiento = a.pkAsiento
                            WHERE v.pkVuelo = idVuelo);
		RETURN personasVuelo;
END
//

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

	IF edad > 3
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
	CREATE TEMPORARY TABLE datos
	SELECT obtener_monto(Reserva.pkReserva) as Monto, Vuelo.pkVuelo AS vuelo
    FROM Reserva inner join Vuelo on Reserva.fkVuelo = Vuelo.pkVuelo
    ORDER BY Monto ASC;
    
    SELECT SUM(Monto) as monto, datos.vuelo
    FROM datos inner join Vuelo on datos.vuelo = Vuelo.pkVuelo
    group by datos.vuelo
    ORDER BY monto ASC
    LIMIT 3;
    
    drop TEMPORARY TABLE datos;
END
//

DELIMITER //
CREATE PROCEDURE editarEdad(idCliente int)
BEGIN
	DECLARE v_edad int;
    SET v_edad = (SELECT esAdulto(idCliente));
    IF v_edad = 0 THEN
		UPDATE Cliente
			SET Edad = 'I' WHERE pkCliente = idCliente;
	END IF;    
END
//

DELIMITER //
CREATE PROCEDURE eliminarReservacion(idReserva INT)
BEGIN
	DECLARE idAsiento int;
    
	DECLARE c1 CURSOR FOR select fkAsiento from ClienteXReserva cXr
		inner join Asiento a on cXr.fkAsiento = a.pkAsiento
		where SUBSTR(a.Tipo, 2, 2) != 'I'
		and cXr.fkReserva = idReserva;
        
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET @hecho = TRUE;

    OPEN c1;
    bucle: LOOP
		FETCH c1 into idAsiento;
        
        IF @hecho THEN
			LEAVE bucle;
		END IF;
        
        UPDATE Asiento SET Estado = 'L' WHERE pkAsiento = idAsiento;
        
	END LOOP;
    CLOSE c1;
    
    DELETE FROM ClienteXReserva WHERE fkReserva = idReserva;
    DELETE FROM Reserva WHERE pkReserva = idReserva;
END
//

DELIMITER //
CREATE PROCEDURE obtenerReservacion(idReserva INT)
BEGIN	
	SELECT r.Fecha, v.pkVuelo, c1.Codigo, v.Salida, c2.Codigo, v.Llegada, 
		(SELECT obtener_monto(idReserva)) Monto, ae.nombre, ae.NumHub, (SELECT obtenerAsientosReserv(idReserva)) Asientos
	FROM Reserva r
	INNER JOIN ClienteXReserva cXr on r.pkReserva = cXr.fkReserva
    INNER JOIN Asiento a on cXr.fkAsiento = a.pkAsiento
    INNER JOIN Vuelo v on a.fkVuelo = v.pkVuelo
    INNER JOIN Ciudad c1 on c1.pkCiudad = v.fkOrigen
    INNER JOIN Ciudad c2 on c2.pkCiudad = v.fkDestino
    JOIN Aerolinea ae
    WHERE r.pkReserva = idReserva;
END
//

DELIMITER //
CREATE FUNCTION obtenerAsientosReserv(idReserva INT)
RETURNS INT
BEGIN
	DECLARE cantidadAsientos INT;
    SET cantidadAsientos = ( SELECT COUNT(v.pkVuelo) 
							 FROM ClienteXReserva cXr
							 INNER JOIN Asiento a on cXr.fkAsiento = a.pkAsiento
							 INNER JOIN Vuelo v on a.fkVuelo = v.pkVuelo
							 WHERE cXr.fkReserva = idReserva
							 AND SUBSTR(a.Tipo, 2, 2) != 'I');
	RETURN cantidadAsientos;
END
//

-- DROPS
drop function asiento_disponible;
drop function AvionVuelo;
drop function crearReservacion;
drop function esAdulto;
drop function existeAvion;
drop function existeCliente;
drop function existeReserva;
drop function existeVuelo;
drop function insertanAvion;
drop function obtenerAsientosReserv;
drop function obtener_monto:
drop function personasXvuelo;
drop function retornaIDAsiento;
drop function verificar_usuario;
drop trigger asientosOcupados;
drop procedure asiento_edades;
drop procedure editarEdad;
drop procedure eliminarReservacion;
drop procedure info_general;
drop procedure inserta_asientos;
drop procedure insertaClientesReserva;
drop procedure obtener_asientos;
drop procedure obtener_query_r;
drop procedure obtener_query_v;
drop procedure obtenerReservacion;
drop procedure tipo_asientos;
drop procedure TOP3_MayorCantidad;
drop procedure TOP3_MayorVenta;
drop procedure verAvion;
