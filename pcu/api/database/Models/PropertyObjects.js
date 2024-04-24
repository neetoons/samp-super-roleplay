module.exports = (sequelize, DataTypes) => {
    return sequelize.define("property_objects",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            create: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            modelid: {
                type: DataTypes.INTEGER,
                allowNull: false
            },
            x: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            y: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            z: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            rx: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            ry: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            rz: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            texture0: {
                type: DataTypes.STRING(256),
                allowNull: true
            },
            texture1: {
                type: DataTypes.STRING(256),
                allowNull: true
            },
            texture2: {
                type: DataTypes.STRING(256),
                allowNull: true
            },
            texture3: {
                type: DataTypes.STRING(256),
                allowNull: true
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};