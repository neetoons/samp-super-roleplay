module.exports = (sequelize, DataTypes) => {
    return sequelize.define("vobjects",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            type: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            name: {
                type: DataTypes.STRING(32),
                allowNull: false
            },
            modelid: {
                type: DataTypes.INTEGER,
                allowNull: false
            },
            off_x: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            off_y: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            off_z: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            rot_x: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            rot_y: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            rot_z: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            attached: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 1
            },
            color0: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            color1: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            color2: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            color3: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            color4: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            text: {
                type: DataTypes.STRING(32),
                allowNull: true
            },
            font: {
                type: DataTypes.STRING(24),
                allowNull: true
            },
            fontsize: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            fontbold: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            fontcolor: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: -1
            },
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};