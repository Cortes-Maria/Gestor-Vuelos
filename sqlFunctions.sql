use gestorVuelos;
-- FUNCION PARA INICIAR SESION
DELIMITER //
CREATE FUNCTION verificar_usuario(v_usuario varchar(20), v_contrasenna varchar(30))
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
                and cXr.fkReserva = idReserva;
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
        
	ELSEIF idQuery = 2 THEN -- Para imprimir asientos
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

-- info general de un vuelo
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
	SET cantCliente = (SELECT COUNT(c.pkCliente)
	FROM Cliente c WHERE c.pkCliente= idCliente);
	IF cantCliente > 0
	THEN RETURN 1;
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

-- DROPS

DROP FUNCTION verificar_usuario;
DROP FUNCTION obtener_monto;
DROP PROCEDURE inserta_asientos;
DROP FUNCTION obtener_query_r;
DROP FUNCTION obtener_query_v;

select * from Cliente;

