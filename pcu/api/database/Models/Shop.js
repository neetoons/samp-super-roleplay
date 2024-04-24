module.exports = (sequelize, DataTypes) => {
    return sequelize.define("shop",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            type: {
                type: DataTypes.INTEGER,
                allowNull: false
            },
            price: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            text: {
                type: DataTypes.STRING(24),
                allowNull: false
            },
            modelid: {
                type: DataTypes.INTEGER,
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
            zoom: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 1.0
            },
            vcol1: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 1
            },
            vcol2: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 1
            },
            date: {
                type: DataTypes.DATE,
                allowNull: false,
                defaultValue: sequelize.literal("CURRENT_TIMESTAMP")
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};