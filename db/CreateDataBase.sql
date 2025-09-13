-- DATABASE POSTGRESQL
-- CREATE DATABASE IF NOT EXISTS roomControl;
-- USE roomControl;

-- Contiene l'informazione sui controlli correnti della stanza 
-- (un solo record aggiornabile)
CREATE TABLE IF NOT EXISTS ctrl (
    id INT PRIMARY KEY DEFAULT 1,
    updateRate FLOAT DEFAULT 1 CHECK (updateRate > 0),
    agentStatus INT,
    secureMode BOOLEAN DEFAULT FALSE
);

INSERT INTO ctrl (id, updateRate, agentStatus, secureMode)
VALUES (1, 1, 3, FALSE);

-- Contiene i dati relativi allo stato della stanza per gli ultimi 7 giorni
CREATE TABLE IF NOT EXISTS roomStatus (
    giorno TIMESTAMP PRIMARY KEY DEFAULT CURRENT_TIMESTAMP,
    someoneInRoom BOOLEAN NOT NULL,
    humidity FLOAT,
    temperature FLOAT
);

-- Contiene la media giornaliera dello stato della stanza per gli ultimi 30 giorni
CREATE TABLE IF NOT EXISTS monthRoomStatus (
    giorno DATE PRIMARY KEY DEFAULT CURRENT_DATE,
    humidity FLOAT,
    temperature FLOAT
);

-- ACCETTARE QUERY COME INPUT
DROP FUNCTION IF EXISTS exec_query(text);
CREATE OR REPLACE FUNCTION exec_query(sql_query text)
RETURNS SETOF jsonb AS $$
BEGIN
    RETURN QUERY EXECUTE 'SELECT to_jsonb(t) FROM (' || sql_query || ') t';
END;
$$ LANGUAGE plpgsql;
-- SELECT * FROM exec_query('SELECT * FROM roomstatus WHERE giorno::date = ''2025-03-26''');

-- EVENTI PERIODICI
CREATE EXTENSION IF NOT EXISTS pg_cron;

CREATE OR REPLACE FUNCTION delete_old_room_status() 
RETURNS void AS $$
BEGIN
    -- Inserisce nella tabella monthRoomStatus la media di temperatura e umidità degli ultimi 7 giorni
    INSERT INTO monthRoomStatus (humidity, temperature, giorno)
    SELECT
        h_medio AS humidity,
        t_medio AS temperature,
        data AS giorno
    FROM (
        SELECT
            DATE(giorno) AS data,
            AVG(temperature) AS t_medio,
            AVG(humidity) AS h_medio
        FROM
            roomstatus
        GROUP BY DATE(giorno)
    ) AS medie_giornaliere
    WHERE data<CURRENT_DATE - INTERVAL '7 days' ;

    -- Elimina i record più vecchi di 7 giorni da roomStatus
    DELETE FROM roomStatus 
    WHERE giorno < CURRENT_DATE - INTERVAL '7 days';
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION delete_old_month_status() RETURNS void AS $$
BEGIN
    DELETE FROM monthRoomStatus WHERE giorno < CURRENT_DATE - INTERVAL '30 days';
END;
$$ LANGUAGE plpgsql;

-- Esegui delete_old_room_status() ogni giorno a mezzanotte
SELECT cron.schedule(
    'delete_old_room_status',  -- Nome del job
    '0 0 * * *',              -- Ogni giorno a mezzanotte
    $$ SELECT delete_old_room_status(); $$
);

-- Esegui delete_old_month_status() ogni giorno a mezzanotte
SELECT cron.schedule(
    'delete_old_month_status', -- Nome del job
    '0 0 * * *',              -- Ogni giorno a mezzanotte
    $$ SELECT delete_old_month_status(); $$
);

SELECT * FROM cron.job;