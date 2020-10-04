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
	where SUBSTRING(t.Tipo,1,1) = a.Tipo
		and SUBSTRING(t.Tipo, 2) = c.Edad
        and cXr.fkReserva = v_idReservacion;
	IF monto IS NULL THEN
		RETURN 0;
	END IF;
    
	RETURN monto;
END
//


-- DROPS
-- DROP FUNCTION verificar_usuario;
-- DROP FUNCTION obtener_monto;
-- DROP PROCEDURE inserta_asientos;