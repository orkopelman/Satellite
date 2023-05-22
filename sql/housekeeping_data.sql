CREATE TABLE comm_table (
    time         BIGINT           NOT NULL,
    ipAddress    INT              NOT NULL,
    port         INT              NOT NULL,
    size         BIGINT           NOT NULL,
    PRIMARY KEY (time)
);


CREATE TABLE adcs_table (
    time              BIGINT         NOT NULL,
    xrotationAngle    FLOAT(25)      NOT NULL,
    yrotationAngle    FLOAT(25)      NOT NULL,
    zrotationAngle    FLOAT(25)      NOT NULL,
    PRIMARY KEY (time)
);


CREATE TABLE eps_table (
    time       BIGINT       NOT NULL,
    battery    FLOAT(25)    NOT NULL,
    voltage    FLOAT(25)    NOT NULL,
    current    FLOAT(25)    NOT NULL,
    PRIMARY KEY (time)
);


CREATE TABLE temp_table (
    time          BIGINT       NOT NULL,
    temperatue    FLOAT(25)    NOT NULL,
    PRIMARY KEY (time)
);


CREATE TABLE sun_table (
    time          BIGINT         NOT NULL,
    xAxisAngle    FLOAT(25)      NOT NULL,
    yAxisAngle    FLOAT(25)      NOT NULL,
    zAxisAngle    FLOAT(25)      NOT NULL,
    PRIMARY KEY (time)
);
