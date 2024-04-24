module.exports = (sequelize, DataTypes) => {
    return sequelize.define("graffiti",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            x: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            y: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            z: {
                type: DataTypes.FLOAT,
                allowNull: false
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
            interior: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            world: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            modelid: {
                type: DataTypes.INTEGER,
                allowNull: false
            },
            text: {
                type: DataTypes.STRING(128),
                allowNull: false,
                defaultValue: ""
            },
            font: {
                type: DataTypes.STRING(24),
                allowNull: false,
                defaultValue: "Arial"
            },
            font_size: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 12
            },
            font_bold: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            font_color: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: -1
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};