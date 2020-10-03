-- RETORNA DATOS DE CADA RESERVACION
select c.Nombre, c.Apellido1, c.Apellido2, cXr.fkReserva Reservación, c.pkCliente Pasaporte, v.pkVuelo Número_vuelo, a.Tipo Tipo_boleto, a.Fila, a.Columna
	from Cliente c
		inner join ClienteXReserva cXr on c.pkCliente = cXr.fkCliente
        inner join Asiento a on cXr.fkAsiento = a.pkAsiento
        inner join Vuelo v on a.fkVuelo = v.pkVuelo;

-- OBTIENE EL MONTO TOTAL DE CADA RESERVACION
select cXr.fkReserva, sum(t.precio) Monto_total
	from Cliente c
		inner join ClienteXReserva cXr on c.pkCliente = cXr.fkCliente
        inner join Asiento a on cXr.fkAsiento = a.pkAsiento
        inner join Vuelo v on a.fkVuelo = v.pkVuelo
        inner join Tarifa t on v.pkVuelo = t.fkVuelo
        where SUBSTRING(t.Tipo,1,1) = a.Tipo
        and SUBSTRING(t.Tipo, 2) = c.Edad
        group by cXr.fkReserva;

-- ELIMINA LOS REGISTROS INGRESADOS DESDE C
delete from Cliente where pkCLiente != 274628 and pkCLiente != 182959;