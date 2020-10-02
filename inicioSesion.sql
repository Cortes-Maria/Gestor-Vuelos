insert into Cliente(pkCliente, Nombre, Apellido1, Apellido2, Sexo, FechaNacimiento, Edad)
values (274628, 'Ignacio', 'Carrera', 'Rodriguez', 'M', '1980-05-22', 'A');
insert into CuentaCliente(fkCliente, Usuario, Contrasenna)
values (274628, 'Igcaro', 1234);

insert into Cliente(pkCliente, Nombre, Apellido1, Apellido2, Sexo, FechaNacimiento, Edad)
values (182959, 'Daniela', 'Montenegro', 'Sanabria', 'F', '1977-02-10', 'A');
insert into CuentaCliente(fkCliente, Usuario, Contrasenna)
values (182959, 'DaniM', 12345);

-- CLIENTE y CuentaCliente

insert into Operario (pkOperario, Usuario, Contrasenna)
values (312, 'mainAdmin', 4321);

-- Operario

drop function verificar_usuario;

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

select verificar_usuario('Isgcaro', '11234');
select verificar_usuario('Igcaro', '1234');
select verificar_usuario('DaniM', '12345');
select verificar_usuario('DaniM', '1234');
select verificar_usuario('admin/mainAdmin', '4321');
select verificar_usuario('admin/mainAdmin', '54321');
select verificar_usuario('admin/mainAdmin2', '4321');
