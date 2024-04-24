module.exports = (sequelize, DataTypes) => {
    return sequelize.define("bad_history",
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
            by: {
                type: DataTypes.INTEGER,
                allowNull: true
            },
            text: {
                type: DataTypes.STRING(128),
                allowNull: false,
                defaultValue: "razón no especificada"
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